// file: /daemon/master.c

#include <globals.h>

inherit "/inherit/master/valid";

/*
 * Definitions for localtime() efun
 */
#define LT_SEC          0
#define LT_MIN          1
#define LT_HOUR         2
#define LT_MDAY         3
#define LT_MON          4
#define LT_YEAR         5
#define LT_WDAY         6
#define LT_YDAY         7
#define LT_GMTOFF       8
#define LT_ZONE         9
#define LT_ISDST        10

#define ERRORLEVEL 2

void log_file(string file, string msg);

//--------------------------------------------------
void flag(string str)
{
    switch (str)
    {
    case "test":
        "/command/tests"->main();
        break;
    case "sprintf":
    {
        string foo = "This is a test.\n";
        string bar = "Of the sprintf benchmark system.\n";
        string bazz = "This is only a test.\n";

        for (int i = 0; i < 10000; i++)
        {
            sprintf(
                "Let's see if %20|s this is any %-100s faster than the %20i old "
                "way.%s",
                foo, bar, 42, bazz);
        }
        shutdown();
        break;
    }
    default:
        write("Unknown flag.\n");
    }
    shutdown();
}

//--------------------------------------------------
object connect()
{
    object login_ob;
    mixed err;

    err = catch (login_ob = new (LOGIN_OB));

    if (err)
    {
        write("It looks like someone is working on the player object.\n");
        write(err);
        destruct(this_object());
    }
    return login_ob;
}

//--------------------------------------------------
// compile_object: This is used for loading MudOS "virtual" objects.
// It should return the object the mudlib wishes to associate with the
// filename named by 'file'.  It should return 0 if no object is to be
// associated.
mixed compile_object(string file)
{
    //    return (mixed)VIRTUAL_D->compile_object(file);
    return 0;
}

//--------------------------------------------------
// This is called when there is a driver segmentation fault or a bus error,
// etc.  As it's static it can't be called by anything but the driver (and
// master).
staticf void crash(string, object, object)
{
    error("foo\n");
    foreach(object ob in users())
    tell_object(ob,
                "Master object shouts: Damn!\nMaster object tells you: The "
                "game is crashing.\n");
#if 0
    log_file("crashes", MUD_NAME + " crashed on: " + ctime(time()) +
        ", error: " + error + "\n");
    if (command_giver) {
        log_file("crashes", "this_player: " + file_name(command_giver) + "\n");
    }
    if (current_object) {
        log_file("crashes", "this_object: " + file_name(current_object) + "\n");
    }
#endif
}

//--------------------------------------------------
// Function name:   update_file
// Description:     reads in a file, ignoring lines that begin with '#'
// Arguements:      file: a string that shows what file to read in.
// Return:          Array of nonblank lines that don't begin with '#'
// Note:            must be declared static (else a security hole)
staticf string *update_file(string file)
{
    string *arr;
    string str;
    int i;

    str = read_file(file);
    if (!str)
    {
        return ({});
    }
    arr = explode(str, "\n");
    for (i = 0; i < sizeof(arr); i++)
    {
        if (arr[i][0] == '#')
        {
            arr[i] = 0;
        }
    }
    return arr;
}

//--------------------------------------------------
// Function name:       epilog
// Return:              List of files to preload
string *epilog(int)
{
    string *items;

    items = update_file(CONFIG_DIR + "/preload");
    return items;
}

//--------------------------------------------------
// preload an object
void preload(string file)
{
    int t1;
    string err;

    if (file_size(file + ".c") == -1)
        return;

    t1 = time();
    write("Preloading : " + file + "...");
    err = catch (call_other(file, "??"));
    if (err != 0)
    {
        write("\nError " + err + " when loading " + file + "\n");
    }
    else
    {
        t1 = time() - t1;
        write("(" + t1 / 60 + "." + t1 % 60 + ")\n");
    }
}

//--------------------------------------------------
// Write an error message into a log file. The error occured in the object
// 'file', giving the error message 'message'.
void log_error(string, string message)
{
    write_file(LOG_DIR + "/compile", message);
}

//--------------------------------------------------
// save_ed_setup and restore_ed_setup are called by the ed to maintain
// individual options settings. These functions are located in the master
// object so that the local admins can decide what strategy they want to use.
int save_ed_setup(object who, int code)
{
    string file;

    if (!intp(code))
    {
        return 0;
    }
#ifdef __PACKAGE_UIDS__
    file = user_path(getuid(who)) + ".edrc";
#else
    file = "/.edrc";
#endif
    rm(file);
    return write_file(file, code + "");
}

//--------------------------------------------------
// Retrieve the ed setup. No meaning to defend this file read from
// unauthorized access.
int retrieve_ed_setup(object who)
{
    string file;
    int code;

#ifdef __PACKAGE_UIDS__
    file = user_path(getuid(who)) + ".edrc";
#else
    file = "/.edrc";
#endif
    if (file_size(file) <= 0)
    {
        return 0;
    }
    sscanf(read_file(file), "%d", code);
    return code;
}

//--------------------------------------------------
// When an object is destructed, this function is called with every
// item in that room.  We get the chance to save users from being destructed.
void destruct_environment_of(object ob)
{
    if (!interactive(ob))
    {
        return;
    }
    tell_object(ob, "The object containing you was dested.\n");
    ob->move(VOID_OB);
}


//--------------------------------------------------
// make_path_absolute: This is called by the driver to resolve path names in ed.
string make_path_absolute(string file)
{
    file = resolve_path((string)this_player()->query_cwd(), file);
    return file;
}

//--------------------------------------------------
string get_root_uid()
{
    return ROOT_UID;
}

//--------------------------------------------------
string get_bb_uid()
{
    return BACKBONE_UID;
}

//--------------------------------------------------
string creator_file(string str)
{
    return (string)call_other(SINGLE_DIR + "/simul_efun", "creator_file", str);
}

//--------------------------------------------------
string domain_file(string str)
{
    return (string)call_other(SINGLE_DIR + "/simul_efun", "domain_file", str);
}

//--------------------------------------------------
string author_file(string str)
{
    return (string)call_other(SINGLE_DIR + "/simul_efun", "author_file", str);
}

//--------------------------------------------------
string privs_file(string f)
{
    return f;
}

//--------------------------------------------------
staticf void error_handler(mapping error, int caught)
{
    object ob;
    string logContent = "---\n";

    ob = this_interactive() || this_user();

    if(caught) logContent += "**Error Caught\n";

    logContent += sprintf("Error: %s\n", error["error"]);

    if(error["object"])
        logContent += sprintf("Current object: %O\n", error["object"]);
    else
        logContent += "No current object.\n";

    if(error["program"])
        logContent += sprintf("Current Program: %O\n", error["program"]);
    else
        logContent += "No current program.\n";

    logContent += sprintf("Line: %5d, File: %s\n\n", error["line"], error["file"]);

    logContent += sprintf("Traceback:\n%s\n\n",
      implode(map_array(
        error["trace"],
        (:
          sprintf(
            "Line: %5d, File: %s\n Object: %O\n Program: %s" +
#if ERRORLEVEL > 1
            "\n Arguments: '%s'" +
#elif ERRORLEVEL > 2
            "\n Locals: '%s'" +
#endif
            "",
              $1["line"],
              $1["file"],
              $1["object"] || "No object.",
              $1["program"] || "No program."
#if ERRORLEVEL > 1
              , ($1["arguments"] ? implode($1["arguments"], ", ") : "No arguments.")
#elif ERRORLEVEL > 2
              , ($1["locals"] ? implode($1["locals"], ", ") : "No locals.")
#endif
          )
        :)
      ), "\n")
    );

    if(ob)
    {
        if(sscanf(error["file"], "/home/%*s/" + ob->query_name() + "/%*s"))
            write_file(user_path(ob->query_name()) + "log", logContent[0..<2]);

        if(!caught) tell_object(ob, logContent);
    }

    log_file("log", logContent);
}

//--------------------------------------------------
void log_file(string file, string msg)
{
    string fname;
    int size;

    //if (query_privs(previous_object()) == "[open]") return;

    msg = ctime(time()) + ":" + msg;
    fname = LOG_DIR + "/" + file;
    size = file_size(fname);

    if (size == -2) return;
    if (size + strlen(msg) >= 20000)
    {
    mixed array ltime = localtime(time());
    string t1;
    string backup;
    int ret = sscanf(file, "%s.log", t1);

    if (ret == 0)
        backup =
              sprintf("archive/%s-%04d%02d%02d%02d%02d%02d",
            file,
                ltime[LT_YEAR],
                ltime[LT_MON] + 1,
            ltime[LT_MDAY],
            ltime[LT_HOUR],
            ltime[LT_MIN],
                ltime[LT_SEC],
          );
    else
        backup =
          sprintf("archive/%s-%02d%02d%02d%02d%02d%02d.log",
            t1,
            ltime[LT_YEAR],
            ltime[LT_MON] + 1,
            ltime[LT_MDAY],
            ltime[LT_HOUR],
            ltime[LT_MIN],
            ltime[LT_SEC],
        );
    rename(fname, LOG_DIR + "/" + backup);
    }

    write_file(fname, msg);
}

int valid_compile_to_c()
{
    return 1;
}
