int directory_exists(string dirname)
{
    return (file_size(dirname) == -2);
}

int file_exists(string file)
{
    return (file_size(file) >= 0);
}

int user_exists(string user)
{
    return (file_exists("/u/" + user + "/" + user + ".o"));
}


