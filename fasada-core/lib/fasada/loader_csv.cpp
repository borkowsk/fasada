/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @footnote Technical mail: <fasada.wb@gmail.com>
/// @copyright 2019
/// @version 0.01
///
///  See CURRENT licence file!
///
#define UNIT_IDENTIFIER "loader_csv"
#include "fasada.hpp"
#include "loader_csv.h"
#include <boost/algorithm/string/replace.hpp> ///https://stackoverflow.com/questions/4643512/replace-substring-with-another-substring-c
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>

using namespace boost::filesystem;

#if(0)
#include <boost/filesystem/string_file.hpp> //only new boost
namespace fasada
{

#else
namespace fasada
{

inline
void load_string_file(const path& p, std::string& str)
{
  ifstream file;
  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  file.open(p, std::ios_base::binary);
  std::size_t sz = static_cast<std::size_t>(file_size(p));
  str.resize(sz, '\0');
  file.read(&str[0], sz);
}
#endif

loader_csv::loader_csv(const char* name):
    loader_processor(name) //also may be READER if it should create its own FORM
{}

void loader_csv::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
//Implement_write WRITER'a powinno zmienić wartości na powstawie FORMularza z method==GET
{
    bool        html=request.asHTML();
    if(html)
    {
        o+=ipc::string(EXT_PRE)+"htm\n";
        o+=getPageHeader(request.getFullPath())+"\n<PRE>\n";
    }

    std::string discPath=request["targetpath"];

    if(discPath=="" || discPath=="...")
            discPath=request["&private_directory"]+request["&path"];

    boost::replace_all(discPath,"//","/");

    if(top.size()!=0  && request["force"]!="true" ) //Jak już jest zawartość to trzeba być pewnym
    {
        //o+="Only leaf type nodes can be modified by "+procName+"\n";
        throw(tree_processor_exception("PTREE PROCESSOR "+procName+" CANNOT LOAD NOT-LEAF NODE!\nUse &force=true if You are sure."));
    }

    //....
    o+="LOADING:";
    o+=discPath;
    o+="\n";

    std::string content;
    //boost::filesystem::
    load_string_file(discPath,content);

    top.put_child("_raw_content",pt::ptree{content});

    //Jeśli nie ma wyjątku to nazwę procesora likwidujemy
    top.data()="";
    _implement_magic_file_test(o,top,discPath,request);
    //utf-8 - OK
    //us-ascii - chyba też OK
    //utf-16le - wymaga przekodowania TODO
    //binary ???? zobacz 'Learning.csv'

    // Most important properties is "source", "loader", "viewer", "saver", "alternative_savers", "oth_actions":
    insert_property(top,"_source",discPath+" ");//SPACE is intentional. It blocs the next CHECKs.
    insert_property(top,"_loadtimestamp",std::to_string(time(NULL)));
    insert_property(top,"viewer","viewCsv");
    insert_property(top,"loader", procName);
    insert_property(top,"saver", "saveAsCsv");
    insert_property(top,"alternative_savers.json","saveAsJson");
    insert_property(top,"alternative_savers.xml","saveAsXml");
    insert_property(top,"oth_actions.decode","rawContentAsCsv");

    o+="DONE";

    if(html)
    {
        o+="\n</PRE>\n";
        o+=getActionLink(request.getFullPath()+"?ls&html&long","LSL","List as long content of "+request["&path"])+"&nbsp;&nbsp; ";
        o+=getPageClosure(_compiled);
    }
}


}
