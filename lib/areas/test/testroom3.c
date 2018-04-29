inherit ROOM_BASE;

void init()
{
    set_name("Test Room Two");
    set_desc("This room is a test room for the room system "
             "and contains various testing equipment. Some of "
             "it seems a little out of place but what do you know.");
    // test remove exit and add exit functionality
    // this should result in north leading to /single/void.c
    add_exit("west", "/areas/test/testroom1.c");
}