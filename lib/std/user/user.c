inherit BASE;

private
string name;
string password;
string cwd;

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

void save()
{
    save_object(user_data_file(query_name()) + ".o");
}

void restore()
{
    restore_object(user_data_file(query_name()) + ".o");
}

// called by the present() efun (and some others) to determine whether
// an object is referred as an 'arg'.

int id(string arg)
{
    return (arg == query_name()) || base::id(arg);
}

// preprocess input
string process_input(string arg)
{
    // possible to modify player input here before driver parses it.
    return arg;
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
		movementHook(cmd_path);
        // maybe call an emote/soul daemon here
    }
    return 0;
}


int movementHook (string arg)
{
	/*
	switch(cmd_path)
	{
		case "n":	cmd_path = "north";
			break;
		
		case "s": 	cmd_path = "south"; 
			break;
		
		case "e":	cmd_path = "east"; 
			break;
		
		case "w":	cmd_path = "west"; 
			break;
		
		case "se":	cmd_path = "southeast"; 
			break;
		
		case "sw":	cmd_path = "southwest"; 
			break;
		
		case "ne":	cmd_path = "northeast"; 
			break;
		
		case "nw":	cmd_path = "northwest"; 
			break;
		
		case "u":	cmd_path = "up"; 
			break;
		
		case "d":	cmd_path = "down"; 
			break;
		
		default:  
			break;
	}
	*/
	
    mapping room_exits = environment(this_player())->query_exits();
	tell_object(this_player(), "Arg : " + arg);
	if( member_array(arg,keys(environment(this_player())->query_exits())) != 1 )
	{
		this_player()->move(room_exits[arg][0]);
		return 1;
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
