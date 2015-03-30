//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2013-2015 Glenn De Jonghe
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.


#ifndef __HEADER_ONLINE_PROFILE_FRIENDS_HPP__
#define __HEADER_ONLINE_PROFILE_FRIENDS_HPP__

#include <string>
#include <irrString.h>

#include "guiengine/screen.hpp"
#include "guiengine/widgets.hpp"
#include "states_screens/online_profile_base.hpp"
#include "online/profile_manager.hpp"


namespace GUIEngine { class Widget; }


/** Online profile overview screen.
 * \ingroup states_screens
 */
class OnlineProfileFriends : public OnlineProfileBase, public GUIEngine::ScreenSingleton<OnlineProfileFriends>
{
private:
    OnlineProfileFriends();

    /** Pointer to the various widgets on the screen. */
    GUIEngine::ListWidget    *m_friends_list_widget;
    GUIEngine::ButtonWidget  *m_search_button_widget;
    GUIEngine::TextBoxWidget *m_search_box_widget;

    bool                        m_waiting_for_friends;

    void displayResults();
public:
    friend class GUIEngine::ScreenSingleton<OnlineProfileFriends>;

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void loadedFromFile() OVERRIDE;

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void eventCallback(GUIEngine::Widget* widget,
                               const std::string& name,
                               const int playerID) OVERRIDE;

    /** \brief implement callback from parent class GUIEngine::Screen */
    virtual void init() OVERRIDE;

    virtual void onUpdate(float delta) OVERRIDE;
    virtual void beforeAddingWidget() OVERRIDE;

    // ------------------------------------------------------------------------
    /** Triggers a reload of the friend list next time this menu is shown. */
    void refreshFriendsList() {m_waiting_for_friends = true; }
};

#endif
