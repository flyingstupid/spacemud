#include <globals.h>

// needs fixed to handle passwords
#define NO_ECHO 1
#define NO_BYPASS 2

#define MAX_NAME 11

void new_character(string arg, object user);
void get_password(string arg, object user, int new_char);
void get_name(string arg);

void logon()
{
    write("Welcome to spacemud!\n\n");
    cat("/etc/motd");
    write("\nEnter a name>\n");
    input_to("get_name", NO_BYPASS);
}

void get_name(string arg)
{
    object user;
    string err;
    string userFile;

    if (!arg || strlen(arg) < 2)
    {
        write("You must input a name> ");
        input_to("get_name", NO_BYPASS);
        return;
    }

    if (strlen(arg) > MAX_NAME)
    {
        write("Maximum name length is 11 characters.\n");
        write("\nEnter a name>\n");
        input_to("get_name", NO_BYPASS);
        return;
    }

    arg = lower_case(arg);

    if (arg == "quit")
    {
        write("See you soon!\n");
        destruct();
        return;
    }

    for (int i = 0; i < strlen(arg); i++)
    {
        if (arg[i] < 'a' || arg[i] > 'z')
        {
            write("Names can only contain letter. '" + arg[i] + "' is not allowed\n");
            write("\nEnter a name>\n");
            input_to("get_name", NO_BYPASS);
            return;
        }
    }

    err = catch (user = new ("/clone/user"));

    if (err)
    {
        write("[error] (login): Unable to create user object.\n");
        destruct();
        return;
    }
    user->set_name(arg);

    userFile = user_data_file(arg) + ".o";

    if (!file_exists(userFile))
    {
        write("New character " + arg + "?\n(Y/n)>");
        input_to("new_character", NO_BYPASS, user);
        return;
    }

    seteuid(getuid(this_object()));
    user->restore();
    write("Enter password> \n");
    input_to("get_password", NO_BYPASS | NO_ECHO, user, 0);
}

void get_password(string arg, object user, int new_char)
{
    string name = user->query_name();

    arg = crypt(arg, arg);

    if (new_char == 1)
    {
        write("Confirm password>\n");
        input_to("get_password", NO_BYPASS | NO_ECHO, user, 2);
        user->set_password(arg);
        return;
    }
    else if (new_char == 2)
    {
        if (arg != user->query_password())
        {
            write("Passwords do not match!\n");
            write("Enter password> \n");
            input_to("get_password", NO_BYPASS | NO_ECHO, user, 1);
            return;
        }

        mkdir(user_data_path(name));
        user->save();
    }
    else if (arg != user->query_password())
    {
        write("Incorrect!\n");
        destruct();
        return;
    }
    exec(user, this_object());
    user->setup();
    user->move(VOID_OB);
    destruct(this_object());
}

void new_character(string arg, object user)
{
    arg = lower_case(arg);

    if (!arg || strlen(arg) == 0 || arg[0] == 'y')
    {
        write("creating character.\n");
        user->set_password("");
        write("\nEnter Password>\n");
        input_to("get_password", NO_BYPASS | NO_ECHO, user, 1);
        return;
    }
    else if (arg[0] == 'n')
    {
        write("Ok then, Bye.\n");
        destruct();
        return;
    }

    write("New character " + arg + "?\n(Y/n)>");
    input_to("new_character", NO_BYPASS, user);
    return;
}
