string user_data_path(string name)
{
    if (!name || !stringp(name))
        return 0;
    name = lower_case((string)name);
    return ("/u/" + name);
}

string user_data_file(string name)
{
    if (!name || !stringp(name))
        return 0;
    name = lower_case((string)name);
    return (user_data_path(name) + "/" + name);
}

