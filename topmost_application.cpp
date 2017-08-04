/*
* window_to_topmost.cpp
* Created on : Jul 13, 2017
* Author : Clint Ferrin
* 
*	Description : This program populates a vector of handers, tiles, and
*	classes for open windows on Microsoft Windows OS.It compares the list to
*	a string given by the user, and places the window in focus on top with
*	HWND_TOPMOST mode - meaning that the window stays on top of all other
*	windows when it is not active.If no window is found with the given name,
*	the program will notify the user via the command line.
*/
#include <Windows.h>
#include <string>
#include <iostream>
#include <ctype.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <cstdlib>

struct handle
{
public:
	handle();
	handle(HWND, std::string, std::string);
	HWND hwnd;
	std::string title;
	std::string class_name;
};

handle::handle()
{
	hwnd = NULL;
	title ="";
	class_name ="";
}

handle::handle(HWND new_hwnd, std::string new_title, std::string new_class_name)
		{
		hwnd = new_hwnd;
		title = new_title;
		class_name = new_class_name;
}

struct list_of_handles
{
public:
	list_of_handles();
	int count;
	std::vector<handle> handles;

};
list_of_handles::list_of_handles()
{
	count = 0;
}
// The CALLBACK function runs through all of the window handlers and inputs them
// into the following code.The parameter lParam can take the form of any variable
// and contains a vector of all of the hwnd with their corresponding title and
// class.
// The function is called using the command :
// EnumWindows(populate_handl ers, (LPARAM)&hwnds);
// Input : All Windows, and l i st of handler's vector
// Output : Filled list_of_handlers struct.
BOOL CALLBACK populate_handles(HWND hwnd, LPARAM lParam)
{
	list_of_handles * hwnds = (list_of_handles*)lParam;
	handle single_handle;
	char title[128];
	char class_name[128];
	GetWindowTextA(hwnd, title, sizeof(title));
	GetClassNameA(hwnd, class_name, sizeof(class_name));
	single_handle.hwnd = hwnd;
	single_handle.title = title;
	single_handle.class_name = class_name;
	hwnds->count++;
	hwnds->handles.push_back(single_handle);
	return TRUE; // FALSE will stop the function at return
}
			
// find_running_window() loops through all ements of list_of_handles.handles and
// compares the titles to the given string.Note that the title and string must
// match exactly to be recognized.
// Input : list_of_handles and string to compare
// Output : HWND(handle) of the mathing window
HWND find_running_window(list_of_handles * hwnds, std::string title)
{
	std::cout << title << std::endl;
	for (int i = 0; i < hwnds->count; i++)
	{
		if (hwnds->handles[i].title.compare(title) == 0)
		{
			return hwnds->handles[i].hwnd;
		}
	}
	std::cout <<"Window \"" << title <<"\" not found." << std::endl;
}

int find_running_window_group(list_of_handles * hwnds, std::vector<HWND> *list, std::string title)
{
	for (int i = 0; i < hwnds->count; i++)
	{
		if (hwnds->handles[i].title.compare(title) == 0)
		{
			list->push_back(hwnds->handles[i].hwnd);
		}
	}
	if (list->size() == 0)
	{
		std::cout <<"Window \"" << title <<"\" not found. 11 << std::endl";
		return 0;
	}
	return 1;
}
void find_running_window_group_input(list_of_handles * hwnds, std::vector<HWND> * list, std::vector<char*> children_names)
{
	for (int i = 0; i < children_names.size(); i++)
	{
		for (int j = 0; j < hwnds->count; j++)
		{
			if (hwnds->handles[j].title.compare(children_names[i]) == 0)
			{
				list->push_back(hwnds->handles[j].hwnd);
			}
		}
	}
	if (list->size() == 0)
	{
		std::cout <<"Child window not found." << std::endl;
		exit(0);
	}
}
handle find_window_by_hwnd(list_of_handles * hwnds, HWND hwnd)
{
	for (int i = 0; i < hwnds->count; i++)
	{
		if ((int*)hwnds->handles[i].hwnd == (int*)hwnd)
		{
			return hwnds->handles[i];
		}
	}
	std::cout <<"Handle \"" << hwnd <<" not found." << std::endl;
}

void print_handle(handle * h_handle)
{
	std::cout <<"T:" << h_handle->title << std::endl;
	std::cout <<"C:" << h_handle->class_name << std::endl;
	std::cout <<"H:" << h_handle->hwnd << std::endl << std::endl;
}


void print_hwnd(HWND hwnd)
{
	handle h_hwnd;
	char title[128];
	char class_name[128];
	GetWindowTextA(hwnd, title, sizeof(title));
	GetClassNameA(hwnd, class_name, sizeof(class_name));
	h_hwnd.title = title;
	h_hwnd.class_name = class_name;
	h_hwnd.hwnd = hwnd;
	print_handle(&h_hwnd);
}

HWND get_foreground_window()
{
	std::cout <<"Select Window";
	std::cout <<".";
	Sleep(1000);
	std::cout <<".";
	Sleep(1000);
	std::cout <<".";
	Sleep(1000);
	std::cout <<"\n\n";
	HWND hwnd = GetForegroundWindow();
	return hwnd;
}
		
void print_all_handles(list_of_handles * hwnds)
{
	for (int i = 0; i << hwnds->count; i++)
	{
		std::cout << i << std::endl;
		print_handle(&hwnds->handles[i]);
	}
}
void set_topmost(HWND hwnd)
{
	SetFocus(hwnd);
	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void v_set_topmost(std::vector<HWND> *windows)
{
	for (int i = 0; i < windows->size(); i++)
	{
		set_topmost((*windows)[i]);
	}
}

void no_topmost(HWND hwnd)
{
	SetFocus(hwnd);
	SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void v_no_topmost(std::vector<HWND> *windows)
{
	for (int i = 0; i < windows->size(); i++)
	{
		no_topmost((*windows)[i]);
	}
}

void write_handles(list_of_handles * hwnds)
{
	char * file_name ="output.txt";
	std::ofstream fout;
	fout.open(file_name);
	for (int i = 0; i < hwnds->count; i++)
	{
		fout << i << std::endl;
		fout <<"T:" << hwnds->handles[i].title <<"\n";
		fout <<"C:" << hwnds->handles[i].class_name <<"\n";
		fout <<"H:" << hwnds->handles[i].hwnd <<"\n\n";
	}

	fout.close();
	std::cout <<"Output written to file \"" << file_name <<".\"" << std::endl;
}

int get_r_width(RECT rect)
{
	return rect.right - rect.left;
}
		   
int get_r_height(RECT rect)
{
	return rect.bottom - rect.top;
}


void print_rect(RECT rect)
{
	std::cout <<"L:" << rect.left << std::endl;
	std::cout <<"R:" << rect.right << std::endl;
	std::cout <<"B:" << rect.bottom << std::endl;
	std::cout <<"T:" << rect.top << std::endl;
	std::cout <<"Dim:" << get_r_width(rect) <<"X" << get_r_height(rect) << std::endl << std::endl;
}

void set_parent_child(HWND parent, HWND child)
{
	RECT r_parent, r_child;
	GetWindowRect(parent, &r_parent);
	GetWindowRect(child, &r_child);

	// set largest window to parent
	if (abs(get_r_height(r_parent) * get_r_width(r_parent)) < abs(get_r_height(r_child) * get_r_width(r_child)))
	{
		HWND tmp;
		tmp = parent;
		parent = child;
		child = tmp;
		GetWindowRect(parent, &r_parent);
		GetWindowRect(child, &r_child);
	}

	// find original ratios for parent and child
	double x_start_ratio = (r_parent.left - r_child.left) / double(get_r_width(r_parent));
	double y_start_ratio = (r_parent.top - r_child.top) / double(get_r_height(r_parent));

	double width_ratio = double(get_r_width(r_parent)) / get_r_width(r_child);
	double height_ratio = double(get_r_height(r_parent)) / get_r_height(r_child);

	// position child in relation to parent
	for (int i = 0; i < 1000000000000; i++)
	{
		GetWindowRect(parent, &r_parent);
		SetWindowPos(child, NULL,
			r_parent.left - x_start_ratio * get_r_width(r_parent),
			r_parent.top - y_start_ratio * get_r_height(r_parent),
			abs(get_r_width(r_parent) / width_ratio),
			abs(get_r_height(r_parent) / height_ratio),
			SWP_ASYNCWINDOWPOS | SWP_NOACTIVATE);
		SetFocus(parent);
	}
}

struct window_ratios
{
public:

	double x_start_ratio;
	double y_start_ratio;
	double width_ratio;
	double height_ratio;
};

void set_parent_children(HWND parent, std::vector<HWND> children)
{
	RECT r_parent;

	std::vector<RECT> r_children;
	GetWindowRect(parent, &r_parent);

	for (int i = 0; i < children.size(); i++)
	{
		RECT r_tmp;
		GetWindowRect(children[i], &r_tmp);
		r_children.push_back(r_tmp);
	}

	// find origina l ratios for parent and children
	std::vector<window_ratios> ratios;
	for (int i = 0; i < children.size(); i++)
	{
		window_ratios tmp_ratios;
		tmp_ratios.x_start_ratio = ((r_parent.left - r_children[i].left) / double(get_r_width(r_parent)));
		tmp_ratios.y_start_ratio = ((r_parent.top - r_children[i].top) / double(get_r_height(r_parent)));
		tmp_ratios.width_ratio = (double(get_r_width(r_parent)) / get_r_width(r_children[i]));
		tmp_ratios.height_ratio = (double(get_r_height(r_parent)) / get_r_height(r_children[i]));
		ratios.push_back(tmp_ratios);
	}

	while (GetWindowRect(parent, &r_parent) != NULL)
	{
		for (int i = 0; i < children.size(); i++)
		{
			SetWindowPos(children[i], NULL,
				r_parent.left - ratios[i].x_start_ratio * get_r_width(r_parent),
				r_parent.top - ratios[i].y_start_ratio * get_r_height(r_parent),
				abs(get_r_width(r_parent) / ratios[i].width_ratio),
				abs(get_r_height(r_parent) / ratios[i].height_ratio),
				NULL);
		}
	}
}

struct user_input
{
	bool top;
	bool normal;
	bool all_normal;
	bool output;
	bool rectangle;
	bool parent_mode;
	bool foreground;
	std::string title;
	std::vector<char*> children_names;
};
void print_usage()
{
	char * h ="Help";
	printf(" -h  %s: Print Usage\n", h);

	char * f ="Foreground Info";
	printf(" -f  %s: Reveal Name, Class, and HWND of window\n", f);

	char * t ="Top";
	printf(" -t  %s: Send window to stay on top of screen by selecting window\n", t);

	char* n ="Normal";
	printf(" -n  %s: Remove window ordering by selecting window\n", n);

	char * a ="All Normal";
	printf(" -an %s: Remove topmost for all windows\n", a);

	char * o ="Output";
	printf(" -o  %s: Send all windows to output.txt\n", o);

	char * r ="Rectangle";
	printf(" -r  %s: Return the Rectangle of current window\n", r);

	char * p ="Parent";
	printf(" -pc %s: Set a parent and children.\n", p);

	char* c ="Child";
	printf(" -c  %s: Specify child\n\n", c);

	printf("Usage Example: .\\window_to_tomost -pc F16 -c MFD -c PFD\n");
}

void check_user_input(int argc, char * argv[], user_input * input)
{
	if (argc < 2)
	{
		print_usage();
		exit(0);
	}

	input->top = FALSE;
	input->normal = FALSE;
	input->all_normal = FALSE;
	input->output = FALSE;
	input->rectangle = FALSE;
	input->parent_mode = FALSE;
	input->foreground = FALSE;

	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i],"-t") == 0)
		{
			input->top = TRUE;
		}
		else if (strcmp(argv[i],"-f") == 0)
		{
			input->foreground = TRUE;
		}
		else if (strcmp(argv[i],"-n") == 0)
		{
			input->normal = TRUE;
		}
		else if (strcmp(argv[i],"-an") == 0)
		{
			input->all_normal = TRUE;
		}
		else if (strcmp(argv[i],"-o") == 0)
		{
			input->output = TRUE;
		}
		else if (strcmp(argv[i],"-r") == 0)
		{
			input->rectangle = TRUE;
		}
		else if (strcmp(argv[i],"-pc") == 0)
		{
			input->parent_mode = TRUE;
			input->title = argv[++i];
		}
		else if (strcmp(argv[i],"-c") == 0)
		{
			input->children_names.push_back(argv[++i]);
		}
		else
		{
			print_usage();
		}
	}
}

void set_sync_topmost(HWND hwnd)
{
	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_ASYNCWINDOWPOS |
		SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
}
		   
int main(int argc, char * argv[])
{
	// get user input
	user_input input;
	check_user_input(argc, argv, &input);

	// populate window list
	list_of_handles hwnds;
	EnumWindows(populate_handles, (LPARAM)&hwnds);

	HWND hwnd;
	handle h_hwnd;

	if (input.foreground == TRUE) {
		std::cout <<"Aquiring Foreground Window." << std::endl;
		hwnd = get_foreground_window();
		h_hwnd = find_window_by_hwnd(&hwnds, hwnd);
		print_handle(&h_hwnd);
	}

	if (input.all_normal == TRUE) 
	{
		std::cout <<"Setting all windows to normal." << std::endl;
		for (size_t i = 0; i < hwnds.count; i++)
		{
			no_topmost(hwnds.handles[i].hwnd);
		}
	}

	if (input.output == TRUE) 
	{
		write_handles(&hwnds);
	}

	if (input.rectangle == TRUE)
	{
		RECT r_win;
		std::cout <<"Getting rectangle of window." << std::endl;
		hwnd = get_foreground_window();
		GetWindowRect(hwnd, &r_win);
		print_rect(r_win);
	}

	if (input.top == TRUE)
	{
		handle h_topmost;
		std::cout <<"Setting window to topmost." << std::endl;
		hwnd = get_foreground_window();
		set_topmost(hwnd);
		print_hwnd(hwnd);	
		SetWindowLong(hwnd, GWL_STYLE, 0);
	}

	if (input.normal == TRUE)
	{
		handle h_topmost;
		std::cout <<"Setting window to normal." << std::endl;
		hwnd = get_foreground_window();
		no_topmost(hwnd);
		print_hwnd(hwnd);
	}


	if (input.parent_mode == TRUE)
	{
		std::cout <<"Setting Parent-Child Relationship." << std::endl;
		// find parent process
		HWND parent;
		parent = find_running_window(&hwnds, input.title);
		set_topmost(parent);

		// find list of children processes and set at topmost
		std::vector<HWND> children_handles;
		std::cout << input.children_names[0] << std::endl;
		find_running_window_group_input(&hwnds, &children_handles, input.children_names);
		v_set_topmost(&children_handles);
		set_parent_children(parent, children_handles);
	}



	return 0;
}
		   
