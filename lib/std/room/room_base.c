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

string render_room()
{
	string out = "";

	out += sprintf("[%s]\n", query_name());
	out += sprintf("%s\n", query_desc());

}
