#include <command.h>

int main(string arg)
{
	if(!arg || arg == "")
	{
		write("Usage: goto <path>\n");
		return 1;
	}

	
	this_player()->move(arg);
    return 1;
}
