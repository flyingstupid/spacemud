#include <command.h>

int main(string arg)
{
    object room;

    if(!arg || arg == "")
    {
        write("Usage: goto <path>\n");
        return 1;
    }

    this_player()->move(arg);
    room = environment(this_player());
    if(room)
    {
        write(room->render_room());
    }
    return 1;
}
