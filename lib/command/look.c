#include <command.h>

int main(string arg)
{
	object room;
	room = environment(this_player());
	if(room)
	{
		write(room->render_room());
	}
    return 1;
}
