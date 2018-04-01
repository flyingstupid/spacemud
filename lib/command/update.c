#include <command.h>

int main(string file)
{
    object obj;

    //TODO need to call resolve_path() and query_cwd()
    if (!file)
        return notify_fail("update what?\n");

    if (obj = find_object(file))
        destruct(obj);

    obj = load_object(file);

	if(obj)
		write("[SUCCESS]\n");
	else
		write("[FAILURE]\n");

    return 1;
}
