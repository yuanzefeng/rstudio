/*
 * SlideNavigationList.cpp
 *
 * Copyright (C) 2009-12 by RStudio, Inc.
 *
 * Unless you have received this program directly from RStudio pursuant
 * to the terms of a commercial license agreement with RStudio, then
 * this program is licensed to you under the terms of version 3 of the
 * GNU Affero General Public License. This program is distributed WITHOUT
 * ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING THOSE OF NON-INFRINGEMENT,
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. Please refer to the
 * AGPL (http://www.gnu.org/licenses/agpl-3.0.txt) for more details.
 *
 */


#include "SlideNavigationList.hpp"

#include <sstream>

#include <boost/foreach.hpp>

#include <session/SessionModuleContext.hpp>

using namespace core;

namespace session {
namespace modules { 
namespace presentation {

SlideNavigationList::SlideNavigationList()
      : allowNavigation_(true),
        allowSlideNavigation_(true),
        index_(0),
        inSubSection_(false),
        hasSections_(false)
{
}

void SlideNavigationList::setNavigationType(const std::string& type)
{
   if (type.empty() || (type == "slides"))
   {
      allowNavigation_ = true;
      allowSlideNavigation_ = true;
   }
   else if (type == "sections")
   {
      allowNavigation_ = true;
      allowSlideNavigation_ = false;
   }
   else if (type == "none")
   {
      allowNavigation_ = false;
      allowSlideNavigation_ = false;
   }
   else
   {
      allowNavigation_ = true;
      allowSlideNavigation_ = true;
      module_context::consoleWriteError("Unknown type '" + type + "' " +
                                        "for navigation field");
   }
}


void SlideNavigationList::add(const Slide& slide)
{
   // if there is no navigation then we only add the first slide
   if (!allowNavigation_)
   {
      if (slides_.empty())
         addSlide(slide.title(), 0, 0);
   }
   else if (!allowSlideNavigation_)
   {
      if (slide.type() == "section")
         addSlide(slide.title(), 0, index_);
   }
   else
   {
      int indent = 0;
      if (slides_.empty())
      {
         inSubSection_ = false;
         indent = 0;
      }
      else if (slide.type() == "section")
      {
         inSubSection_ = false;
         indent = 0;
         hasSections_ = true;
      }
      else if (slide.type() == "sub-section")
      {
         inSubSection_ = true;
         indent = 1;
         hasSections_ = true;
      }
      else
      {
         indent = inSubSection_ ? 2 : 1;
      }

      addSlide(slide.title(), indent, index_);
   }

   index_++;
}

void SlideNavigationList::complete()
{
   // if we don't have any sections then flatted the indents
   if (!hasSections_)
   {
      BOOST_FOREACH(json::Value& slide, slides_)
      {
         slide.get_obj()["indent"] = 0;
      }
   }
}

std::string SlideNavigationList::asCall() const
{
   std::ostringstream ostr;
   ostr << "window.parent.initPresentationNavigator(";
   json::write(slides_, ostr);
   ostr << ");";
   return ostr.str();
}


void SlideNavigationList::addSlide(const std::string& title,
                                   int indent,
                                   int index)
{
   json::Object slideJson;
   slideJson["title"] = title;
   slideJson["indent"] = indent;
   slideJson["index"] = index;
   slides_.push_back(slideJson);
}

} // namespace presentation
} // namespace modules
} // namesapce session
