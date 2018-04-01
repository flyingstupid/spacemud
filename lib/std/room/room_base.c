string name;
string desc;
mapping exits;

// name
string query_name();
void set_name(string name);

// desc
string query_desc();
void set_desc(string _desc);

// exits
int add_exit(string exit, string path);
int remove_exit(string exit);

// reder methods
string render_room();

//////////////////////////////////////////////////
// Definitions

//--------------------------------------------------
string query_name()
{
    return name;
}

//--------------------------------------------------
string query_desc()
{
    return desc;
}

//--------------------------------------------------
void set_desc(string _desc)
{
    desc = _desc;
}

//--------------------------------------------------
void set_name(string _name)
{
    name = _name;
}

//--------------------------------------------------
int remove_exit(string exit)
{
    if (member_array(exit, keys(exits)) >= 0)
	{
        map_delete(exits, exit);
		return 1;
	}
	return 0;
}

//--------------------------------------------------
int add_exit(string exit, string path)
{
	if(!exits) exits = ([]);
    // don't allow overwriting exits
    if (member_array(exit, keys(exits)) >= 0)
        return 0;

    exits[exit] = ({path});

    return 1;
}

//--------------------------------------------------
mapping query_exits()
{
	return ([]) + exits;
}

//--------------------------------------------------
string render_room()
{
    string out = "";
    int i;
    int numExits;
    array exitKeys;

    out += sprintf("[%s]\n", query_name());
    out += sprintf("%s\n", query_desc());

    exitKeys = keys(exits);
    numExits = sizeof(exitKeys);
    if (numExits == 0)
        out += "No Exits\n";
    else if (numExits == 1)
        out += "Exit: ";
    else
        out += "Exits: ";

    for (i = 0; i < sizeof(exitKeys); ++i)
    {
        out += "[" + exitKeys[i] + "] ";
    }

    return out + "\n";
}
