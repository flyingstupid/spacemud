inherit BASE;

private
string name;
string password;
string cwd;

mapping aliasmap;
mapping envmap;

// replace this with a functioning version.

string query_cwd()
{
    return cwd;
}

// logon: move this to /single/login.c when login.c gets written.

void logon()
{
    write("Welcome to Lil.\n> ");
}

// query_name: called by various objects needing to know this user's name.

string query_name()
{
    return name;
}

void set_name(string arg)
{
    // for now only the login file can set a user's name
    if (explode(file_name(this_player()), "#")[0] == "/clone/login")
        name = arg;
}

string query_password()
{
    return password;
}

void set_password(string arg)
{
    password = arg;
}

void set_env(string key, mixed value)
{
    if(!envmap)
        envmap = ([]);
    envmap[key] = value;
}

mixed query_env(string key)
{
    if(!envmap || !member_array(key, keys(envmap)) == -1)
        return 0;
    return envmap[key];
}

void remove_env(string key)
{
    map_delete(envmap, key);
}

void save()
{
    save_object(user_data_file(query_name()) + ".o");
}

void restore()
{
    restore_object(user_data_file(query_name()) + ".o");
   
    //temp
    if(!aliasmap)
    {
        aliasmap = ([]);
        aliasmap += ([ "n":"north" ]);
        aliasmap += ([ "s":"south" ]);
        aliasmap += ([ "e":"east" ]);
        aliasmap += ([ "w":"west" ]);
        aliasmap += ([ "ne":"northeast" ]);
        aliasmap += ([ "se":"southeast" ]);
        aliasmap += ([ "nw":"northwest" ]);
        aliasmap += ([ "sw":"southwest" ]);
    }
}

// called by the present() efun (and some others) to determine whether
// an object is referred as an 'arg'.

int id(string arg)
{
    return (arg == query_name()) || base::id(arg);
}

// preprocess input
string process_input(string input)
{
    string verb;
    string arg;

    if(sscanf(input, "%s %s", verb, arg) != 2)
    {
        verb = input;
        arg = 0;
    }
   
    if(query_env("alias_debug"))
    {
        tell_object(this_player(), "#######PreProcess#######\n");
        tell_object(this_player(), "Verb: " + verb + "\n");
        tell_object(this_player(), "Arg: " + arg + "\n");
    }

    if(aliasmap && member_array(verb, keys(aliasmap)) != -1)
    {
        verb = aliasmap[verb];
    }

    if(query_env("alias_debug"))
    {
        tell_object(this_player(), "#######PostProcess#######\n");
        tell_object(this_player(), "Verb: " + verb + "\n");
        tell_object(this_player(), "Arg: " + arg + "\n");
    }

    if(arg)
        return verb + " " + arg;

    return verb;
}

int movementHook (string arg)
{
    string cmd = query_verb();
    object room;
    mapping roomExitsArray = environment(this_player())->query_exits();
    if( member_array(cmd,keys(environment(this_player())->query_exits())) != -1 )
    {
        this_player()->move(roomExitsArray[cmd][0]);
        room = environment(this_player());
        if(room)
        {
           tell_object(this_player(),room->render_room());
        }
        return 1;
    }
    return 0;
}


int commandHook(string arg)
{
    string cmd_path;
    object cobj;

    cmd_path = COMMAND_PREFIX + query_verb();

    cobj = load_object(cmd_path);
    if (cobj)
    {
        return (int)cobj->main(arg);
    }
    else
    {
        // Would just make this return the func, but if the soul daemon
        // needs a hook, better make it inside an if. 
        if(movementHook(cmd_path))
        {
            return 1;
        }
        // maybe call an emote/soul daemon here
    }
    return 0;
}


int helpHook(string arg)
{
    string cmd_path;
    object cobj;

    cmd_path = COMMAND_PREFIX + query_verb();

    cobj = load_object(cmd_path);
    if (cobj)
    {
        return (int)cobj->help(arg);
    }
    else
    {
        // maybe call an emote/soul daemon here
    }
    return 0;
}

// init: called by the driver to give the object a chance to add some
// actions (see the MudOS "applies" documentation for a better description).

void init()
{
    // using "" as the second argument to add_action() causes the driver
    // to call commandHook() for those user inputs not matched by other
    // add_action defined commands (thus 'commandHook' becomes the default
    // action for those verbs without an explicitly associated action).
    if (this_object() == this_player())
    {
        add_action("helpHook", "help", 1);
        add_action("commandHook", "", 1);
    }
   
}

// create: called by the driver after an object is compiled.

void create()
{
    seteuid(0); // so that login.c can export uid to us
}

// receive_message: called by the message() efun.

void receive_message(string newclass, string msg)
{
    // the meaning of 'class' is at the mudlib's discretion
    receive(msg);
}

// setup: used to configure attributes that aren't known by this_object()
// at create() time such as living_name (and so can't be done in create()).

void setup()
{
    set_heart_beat(1);
    seteuid(getuid(this_object()));
    set_living_name(query_name());
    enable_commands();
    add_action("helpHook", "help", 1);
    add_action("commandHook", "", 1);
    if(!cwd || cwd == "")
        cwd = user_data_path(query_name()) + "/";
}

// net_dead: called by the gamedriver when an interactive player loses
// hir network connection to the mud.

void tell_room(object ob, string msg)
{
    foreach(ob in all_inventory(ob) - ({ this_object() }))
    tell_object(ob, msg);
}

void net_dead()
{
    set_heart_beat(0);
    tell_room(environment(), query_name() + " is link-dead.\n");
}

// reconnect: called by the login.c object when a netdead player reconnects.

void reconnect()
{
    set_heart_beat(1);
    tell_room(environment(), "Reconnected.\n");
    tell_room(environment(), query_name() + " has reconnected.\n");
}
