// Copyright eeGeo Ltd (2012-2014), All Rights Reserved

#pragma once

#include <string>
#import <UIKit/UIKit.h>
#include "Types.h"

namespace ExampleApp
{
	namespace Helpers
	{
		namespace ImageHelpers
		{
			enum Offset
			{
				Centre = 0,

				Left = 1 << 0,
				Right = 1 << 1,
				Top = 1 << 2,
				Bottom = 1 << 3,

				LeftOf = 1 << 4,
				RightOf = 1 << 5,
				Above = 1 << 6,
				Below = 1 << 7,
			};

			typedef u32 OffsetValue;

			CGRect AddPngImageToParentView(UIView* pParentView, const std::string& name, OffsetValue offsetInParent);

			CGRect AddPngImageToParentView(UIView* pParentView, const std::string& name, float x, float y, float w, float h);

			CGRect AddPngHighlightedImageToParentView(UIView* pParentView, const std::string& name, const std::string& highlightedName, OffsetValue offsetInParent);
		}
	}
}