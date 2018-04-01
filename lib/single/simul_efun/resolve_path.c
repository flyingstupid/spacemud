/* resolve_path.c

 Kenon @ LPUniversity
 06-Oct-06 
 Resolve path simul-efun

*/

/* Last updated by Tacitus on October 6th, 2006 */

string resolve_path(string Current, string Next)
{
   int index;
   int temp;
   string* pathSegments;

   if(Next=="here")
      return file_name(environment(this_player())) + ".c";

   if(Next[0]=='~')
   {
      if((Next[1]=='/')||(sizeof(Next)==1))
         Next=user_path(this_player()->query_name())+Next[1..<1];
      else
      {
         index=strsrch(Next, "/");
         if(index==-1) return user_path(Next[1..<0]);
         else Next=user_path(Next[1..index-1])+Next[index..<0];
      }
   }

   if (Next[0]!='/')  Next=Current+"/"+Next;
   pathSegments = explode(Next,"/");

   for (index=0;index<sizeof(pathSegments);index++)
   {
      if (pathSegments[index] == ".") pathSegments[index] = "";

      if (pathSegments[index] == "..")
      {
         pathSegments[index] = "";
         temp = index-1;
         
         while((temp>=0)&&(!sizeof(pathSegments[temp])))
            temp--;

         if (temp!=-1) pathSegments[temp]="";
      }
   }

   if (Next[<1]=='/') Next="/"+implode(pathSegments, "/")+"/";
   else Next= "/" + implode(pathSegments, "/");

   while(strsrch(Next,"//")!=-1) Next = replace_string(Next,"//","/");

   return Next;
}
