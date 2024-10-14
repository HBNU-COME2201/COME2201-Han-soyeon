#include <iostream>
#include "agent.hpp"
#include "mmanager.hpp"
#include "stochastic_decorator.hpp"
#include "simple_decorator.hpp"

#include <vector>
#include <string>
#include "tinyxml2.h"
#include "special_agent.hpp"

class InitManager
{
public: 
    // 기본 생성자 및 파일 경로를 받아서 XML 파일을 읽어들이는 생성자
    InitManager(){}
    InitManager(std::string path){ 
        tinyxml2::XMLDocument doc;
        doc.LoadFile( path.c_str() ); // XML 파일 로드

        // XML 파일 내의 "scenario" -> "AgentList" 노드 찾기
        tinyxml2::XMLElement* agListElem = doc.FirstChildElement( "scenario" )->FirstChildElement( "AgentList" );

        double x, y, heading, speed, drange;
        for (tinyxml2::XMLElement* agElem = agListElem->FirstChildElement();
            agElem != NULL; agElem = agElem->NextSiblingElement())
            {
                // 각 Agent 엘리먼트로부터 x, y, heading, speed, drange 값을 읽음
                agElem->QueryDoubleAttribute("x", &x);
                agElem->QueryDoubleAttribute("y", &y);
                agElem->QueryDoubleAttribute("heading", &heading);
                agElem->QueryDoubleAttribute("speed", &speed);
                agElem->QueryDoubleAttribute("drange", &drange);

                // 읽어들인 값으로 CSpecialAgent 객체 생성 및 리스트에 추가
                CAgent* ag = new CSpecialAgent(x,  y, heading, speed, drange);
                m_agent_list.push_back(ag);
            }
    }
private:
    std::vector<CAgent*> m_agent_list; // Agent 객체 리스트

public:
    // Agent 리스트를 반환하는 함수
    std::vector<CAgent*>& get_agent_list() { return m_agent_list; }
};

int main(int argc, char** argv)
{
    // InitManager를 이용해 XML 파일로부터 Agent 정보를 읽어들여 객체를 생성
    InitManager init_manager("test.xml");

    // CManeuverManager 객체 생성
    CManeuverManager mmanager;

    // 생성된 Agent 리스트를 이용해 각 Agent를 ManeuverManager에 등록
    for(std::vector<CAgent*>::iterator iter = init_manager.get_agent_list().begin();
        iter != init_manager.get_agent_list().end(); ++iter)
    {
        mmanager.register_publisher(*iter); // 다형성을 이용하여 CAgent의 자식 클래스도 동작 가능
    }

    // 시뮬레이션 시간 루프
    double sim_time;
    double time_step = 1;
    for(sim_time = 0; sim_time < 30; sim_time += time_step)
    {   
        // ManeuverManager의 svc 함수 호출로 각 Agent의 행동을 처리
        mmanager.svc(time_step);

        // 현재 시간과 각 Agent 상태 출력
        std::cout << "----" << std::endl;
        for(std::vector<CAgent*>::iterator iter = init_manager.get_agent_list().begin();
        iter != init_manager.get_agent_list().end(); ++iter)
        {
            std::cout << "Time: " << sim_time << ", " <<*(*iter) << std::endl;
        }
    }
    return 0;
}
