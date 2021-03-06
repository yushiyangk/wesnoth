/*
   Copyright (C) 2014 by Chris Beck <render787@gmail.com>
   Part of the Battle for Wesnoth Project http://www.wesnoth.org/

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef GUI_DIALOGS_LUA_INT_HPP_INCLUDED
#define GUI_DIALOGS_LUA_INT_HPP_INCLUDED

#include "gui/dialogs/dialog.hpp"
#include "gui/widgets/button.hpp"
#include "gui/widgets/scroll_label.hpp"
#include "gui/widgets/text_box.hpp"

#include <boost/scoped_ptr.hpp>

class lua_kernel_base;

namespace gui2
{

class tlua_interpreter : public tdialog
{
public:
	class model;

	tlua_interpreter(lua_kernel_base & lk);

	/** Inherited from tdialog. */
	twindow* build_window(CVideo& video);

	/** Inherited from tdialog. */
	void pre_show(CVideo& video, twindow& window);

	enum WHICH_KERNEL { APP, GAME };
	static void display(CVideo& video, lua_kernel_base * lk);
	static void display(CVideo& video, WHICH_KERNEL which);
private:
	boost::scoped_ptr<model> model_;

	/** Inherited from tdialog, implemented by REGISTER_DIALOG. */
	virtual const std::string& window_id() const;

	void bind(twindow& window);

	void update_contents();

	void handle_copy_button_clicked(twindow & window);

	void input_keypress_callback(bool& handled,
						   bool& halt,
						   const SDLKey key,
						   twindow& window);

	tscroll_label* msg_label;
	tbutton* copy_button;
	ttext_box* text_entry;

	std::string text_entry_;
};

}

#endif /* ! GUI_DIALOGS_CHAT_LOG_HPP_INCLUDED */
