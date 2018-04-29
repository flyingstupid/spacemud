inherit ROOM_BASE;

void init()
{
    set_name("Test Room One");
    set_desc("This room is a test room for the room system "
             "and contains various testing equipment. Some of "
             "it seems a little out of place but what do you know.");
    // test remove exit and add exit functionality
    // this should result in north leading to /single/void.c
    add_exit("north", "/single/void2.c");
    remove_exit("north");
    add_exit("north", "/areas/test/testroom2.c");
    add_exit("east","/areas/test/testroom3.c");
}
