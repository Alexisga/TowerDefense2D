#include "CsvLogger.h"
#include <atomic>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <ctime>
CsvLogger::CsvLogger(std::filesystem::path p):path_(std::move(p)){}
const char* CsvLogger::Header(){return "fecha_hora,partida_id,tiempo_activo_s,evento,oleada,vida_base,monedas,enemigos_eliminados,resultado,detalle";}
std::string CsvLogger::Escape(const std::string& s) {
    if(s.find_first_of(",\"\r\n")==std::string::npos) return s;
    std::string out="\"";for(char c:s){out+=c;if(c=='\"')out+='\"';}return out+'\"';
}
std::string CsvLogger::Timestamp(){
    auto now=std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());std::tm utc{};gmtime_s(&utc,&now);
    std::ostringstream s;s<<std::put_time(&utc,"%Y-%m-%dT%H:%M:%SZ");return s.str();
}
std::string CsvLogger::NewMatchId(){
    static std::atomic<unsigned> n{0};
    return std::to_string(std::chrono::system_clock::now().time_since_epoch().count())+"-"+std::to_string(++n);
}
bool CsvLogger::Write(const LogEvent& e) {
    try {
        if(!path_.parent_path().empty())std::filesystem::create_directories(path_.parent_path());
        bool fresh=!std::filesystem::exists(path_) || std::filesystem::file_size(path_)==0;
        if(!fresh) {
            std::ifstream in(path_,std::ios::binary);std::string line;std::getline(in,line);
            if(!line.empty() && line.back()=='\r')line.pop_back();
            if(!in || line!=Header()) {error_="CSV incompatible: encabezado incorrecto";return false;}
        }
        std::ofstream out(path_,std::ios::app|std::ios::binary);
        if(!out){error_="No se pudo abrir el registro CSV";return false;}
        if(fresh)out<<Header()<<'\n';
        out<<Timestamp()<<','<<Escape(e.matchId)<<','<<std::fixed<<std::setprecision(3)<<e.activeTime<<','<<Escape(e.event)<<','
            <<e.wave<<','<<e.baseLife<<','<<e.coins<<','<<e.kills<<','<<Escape(e.result)<<','<<Escape(e.detail)<<'\n';
        out.flush();if(!out){error_="No se pudo escribir el registro CSV";return false;}
        error_.clear();return true;
    }catch(const std::exception&){error_="Error de acceso al archivo CSV";return false;}
}
