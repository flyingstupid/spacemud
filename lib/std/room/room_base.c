string name;
string desc;
mapping exits;


string query_name()
{
	return name;
}

string query_desc()
{
	return desc;
}

void set_desc(string _desc)
{
	desc = _desc;
}

void set_name(string name)
{
	name = name;
}

int add_exit(string exit, string path)
{
	int ret = 0;
	
	if(member_array(exit, keys(exits)))
	{	
		ret = 1;
	}

	exits[exit] = ({path});
}

string render_room()
{
	string out = "";

	out += sprintf("[%s]\n", query_name());
	out += sprintf("%s\n", query_desc());

}
