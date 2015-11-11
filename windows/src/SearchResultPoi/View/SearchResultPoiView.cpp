// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#include "SearchResultPoiController.h"
#include "SearchResultPoiView.h"
#include "WindowsAppThreadAssertionMacros.h"
#include "YelpSearchJsonParser.h"

#include "ReflectionHelpers.h"

using namespace ExampleApp::Helpers::ReflectionHelpers;

using namespace System;
using namespace System::Reflection;

namespace ExampleApp
{
    namespace SearchResultPoi
    {
        namespace View
        {
            SearchResultPoiView::SearchResultPoiView(WindowsNativeState& nativeState)
                : m_nativeState(nativeState)
            {
            }

            SearchResultPoiView::~SearchResultPoiView()
            {
            }

            void SearchResultPoiView::Show(const Search::SdkModel::SearchResultModel model, bool isPinned)
            {
                m_model = model;
                CreateVendorSpecificPoiView(m_model.GetVendor());

                m_yelpModel = Search::Yelp::SdkModel::TransformToYelpSearchResult(model);

                const std::vector<std::string>& humanReadableCategories(model.GetHumanReadableCategories());
                array<System::String^>^ humanCategoriesArray = gcnew array<System::String^>(humanReadableCategories.size());

                for(size_t i = 0; i < humanReadableCategories.size(); ++ i)
                {
                    const std::string& categoryString(humanReadableCategories[i]);
                    humanCategoriesArray[i] = ConvertUTF8ToManagedString(categoryString);
                }

                const std::vector<std::string>& yelpReviews(m_yelpModel.GetReviews());
                array<System::String^>^ yelpReviewsArray = gcnew array<System::String^>(yelpReviews.size());

                for (size_t i = 0; i < yelpReviews.size(); ++i)
                {
                    const std::string& reviewString(yelpReviews[i]);
                    yelpReviewsArray[i] = ConvertUTF8ToManagedString(reviewString);
                }

                DisplayPoiInfo(ConvertUTF8ToManagedString(model.GetTitle()), ConvertUTF8ToManagedString(model.GetSubtitle()), ConvertUTF8ToManagedString(m_yelpModel.GetPhone())
                                , ConvertUTF8ToManagedString(m_yelpModel.GetWebUrl()), ConvertUTF8ToManagedString(model.GetCategory()), humanCategoriesArray, ConvertUTF8ToManagedString(m_yelpModel.GetImageUrl())
                                , ConvertUTF8ToManagedString(m_yelpModel.GetRatingImageUrl()), ConvertUTF8ToManagedString(model.GetVendor()), yelpReviewsArray, m_yelpModel.GetReviewCount(), isPinned);
            }

            void SearchResultPoiView::Hide()
            {
                DismissPoiInfo();
            }

            void SearchResultPoiView::UpdateImage(const std::string& url, bool hasImage, const std::vector<unsigned char>* pImageBytes)
            {
                array<System::Byte>^ imageDataArray = gcnew array<System::Byte>(pImageBytes->size());

                for (size_t i = 0; i < pImageBytes->size(); ++i)
                {
                    imageDataArray[i] = System::Byte(pImageBytes->at(i));
                }

                UpdateImageData(gcnew System::String(url.c_str()), hasImage, imageDataArray);
            }

            void SearchResultPoiView::InsertClosedCallback(Eegeo::Helpers::ICallback0& callback)
            {
                m_closedCallbacks.AddCallback(callback);
            }

            void SearchResultPoiView::RemoveClosedCallback(Eegeo::Helpers::ICallback0& callback)
            {
                ASSERT_UI_THREAD

                m_closedCallbacks.RemoveCallback(callback);
            }

            void SearchResultPoiView::HandleCloseClicked()
            {
                m_closedCallbacks.ExecuteCallbacks();
            }

            void SearchResultPoiView::InsertTogglePinnedCallback(Eegeo::Helpers::ICallback1<Search::SdkModel::SearchResultModel>& callback)
            {
                ASSERT_UI_THREAD

                m_togglePinClickedCallbacks.AddCallback(callback);
            }

            void SearchResultPoiView::RemoveTogglePinnedCallback(Eegeo::Helpers::ICallback1<Search::SdkModel::SearchResultModel>& callback)
            {
                ASSERT_UI_THREAD

                m_togglePinClickedCallbacks.RemoveCallback(callback);
            }

            void SearchResultPoiView::HandlePinToggleClicked()
            {
                ASSERT_UI_THREAD

                m_togglePinClickedCallbacks.ExecuteCallbacks(m_model);
            }

            void SearchResultPoiView::CreateVendorSpecificPoiView(const std::string& vendor)
            {
                ASSERT_UI_THREAD

                std::string viewClass = "";

                if(vendor == "Yelp")
                {
                    m_uiViewClass = GetTypeFromAssembly("ExampleAppWPF", "ExampleAppWPF.YelpSearchResultsPoiView");
                    ConstructorInfo^ ctor = m_uiViewClass->GetConstructor(CreateTypes(IntPtr::typeid));
                    m_uiView = ctor->Invoke(CreateObjects(gcnew IntPtr(this)));

                    DisplayPoiInfo.SetupMethod(m_uiViewClass, m_uiView, "DisplayPoiInfo");
                    DismissPoiInfo.SetupMethod(m_uiViewClass, m_uiView, "DismissPoiInfo");
                    UpdateImageData.SetupMethod(m_uiViewClass, m_uiView, "UpdateImageData");
                }
                else if(vendor == "DeCarta")
                {
                    viewClass = "com/eegeo/searchresultpoiview/DeCartaSearchResultPoiView";
                }
                else if(vendor == "GeoNames")
                {
                    viewClass = "com/eegeo/searchresultpoiview/GeoNamesSearchResultPoiView";
                }
                else
                {
                    Eegeo_ASSERT(false, "Unknown POI vendor %s, cannot create view instance.\n", vendor.c_str());
                }


            }
        }
    }
}