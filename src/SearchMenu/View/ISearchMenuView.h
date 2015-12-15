// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#pragma once

namespace ExampleApp
{
    namespace SearchMenu
    {
        namespace View
        {
            class ISearchMenuView
            {
            public:
                virtual ~ISearchMenuView() { }
                
                virtual void RemoveSeachKeyboard() = 0;
                
                virtual void DisableEditText() = 0;
                
                virtual void EnableEditText() = 0;
                
                virtual void SetEditText(const std::string& searchText, bool isCategory) = 0;
                
                virtual void CollapseAll() = 0;
                
                virtual void InsertSearchPeformedCallback(Eegeo::Helpers::ICallback1<const std::string&>& callback) = 0;
                virtual void RemoveSearchPeformedCallback(Eegeo::Helpers::ICallback1<const std::string&>& callback) = 0;
                
                virtual void InsertSearchClearedCallback(Eegeo::Helpers::ICallback0& callback) = 0;
                virtual void RemoveSearchClearedCallback(Eegeo::Helpers::ICallback0& callback) = 0;
            };
        }
    }
}