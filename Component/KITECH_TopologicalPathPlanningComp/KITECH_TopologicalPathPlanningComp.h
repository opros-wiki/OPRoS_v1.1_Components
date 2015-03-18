#ifndef _KITECH_TopologicalPathPlanningComp_COMPONENT_H
#define _KITECH_TopologicalPathPlanningComp_COMPONENT_H
/*
 *  Generated sources by OPRoS Component Generator (OCG V2)
 *   
 */
#include <Component.h>
#include <InputDataPort.h>
#include <OutputDataPort.h>
#include <InputEventPort.h>
#include <OutputEventPort.h>
#include <Event.h>
#include <OPRoSTypes.h>

#include "PathPlanningServiceProvided.h"
#include "GlobalMapServiceRequired.h"

using namespace std;
typedef unsigned char BYTE;


struct sTpNode;
struct sTpRoute {
	double distance;
	sTpNode *connectedNode;
};

struct sTpNode {
	double x, y;
	int timeElasped;
	sTpNode *cameFrom;
	vector<sTpRoute> route;

	sTpNode (double _x, double _y) : x(_x), y(_y), timeElasped(0), cameFrom(NULL) { }
};

/**
*	KITECH_TopologicalPathPlanningComp는 이동 로봇의 주행을 위한 전역 경로를 게획하는 컴포넌트이다.
*	KITECH_TopologicalPathPlanningComp는 전역 경로를 계획하는 컴포넌트로써 경로 계획을 위한 표준 인터페이스인 IPathPlanningCompService를 상속받아 구현된다.
*	Topological로 형성된 전역맵 데이터를 사용하여 시작 위치에서 목적지 위치까지 경로를 계획한다.
*	동적 라이브러리로 생성되며 KITECH_TopologicalGlobalMapComp와 연동하여 사용된다.
*/
class KITECH_TopologicalPathPlanningComp: public Component, public IPathPlanningService
{
protected:
// method for required
	GlobalMapServiceRequired *ptrGlobalMapService;

public:
	KITECH_TopologicalPathPlanningComp();
	KITECH_TopologicalPathPlanningComp(const std::string &compId);
	virtual ~KITECH_TopologicalPathPlanningComp();
	virtual void portSetup();

protected:
	virtual ReturnType onInitialize();
	virtual ReturnType onStart();
	virtual ReturnType onStop();
	virtual ReturnType onReset();
	virtual ReturnType onError();
	virtual ReturnType onRecover();
	virtual ReturnType onDestroy();

public:
	virtual ReturnType onEvent(Event *evt);
	virtual ReturnType onExecute();
	virtual ReturnType onUpdated();
	virtual ReturnType onPeriodChanged();

	/**
	 *	파리미터를 설정하는 표준 함수이다.
	 *	@param parameter 파라미터 변수.
	 *	@return true - 파리미터 설정 성공<br>
	 *			false - 파라미터 설정 실패<br>
	 */
	virtual bool SetParameter(Property parameter);

	/**
	 *	파리미터를 얻는 표준 함수이다.
	 *	@return parameter 파라미터를 얻기 위한 포인터 변수<br>
	 */
	virtual Property GetParameter();

	virtual int GetError();

	/**
	 *	로봇의 현재 위치에서 목적지 위치까지 최단 거리의 경로를 생성하는 표준 함수이다.
	 *	@param startPosition 출발 위치.
	 *	@param endPoaition 도착 위치.
	 *	@param path 생성된 경로 데이터.
	 *	@return true - 경로 계획 성공<br>
	 *	        false - 경로 계획 실패<br>
	 */
	virtual vector<ObjectLocation> FindPath(ObjectLocation startPosition,ObjectLocation endPosition);

private:
	/**
	 *	파라미터를 저장하는 변수이다.
	 *	본 컴포넌트에서는 사용하지 않는다.
	 */
	Property parameter;

	int error;

	/**
	 *	위상학적 연결 정보를 저장하는 변수
	 */
	vector<sTpNode> topologicalMap;

	/**
	 *	입력되는 맵 데이터에서 해당 위치의 맵 정보를 얻는다.
	 *	@param image 맵 데이터
	 *	@param width 맵의 폭
	 *	@param height 맵의 높이
	 *	@param x 가로축 위치
	 *	@param y 세로축 위치
	 *	@return 맵 정보<br>
	 */
	inline BYTE GetPixel (BYTE *image, int width, int height, int x, int y)	{ return *(image + width * y + x); }

	/**
	 *	입력되는 맵 데이터에 해당 위치의 맵 정보를 저장한다.
	 *	@param image 맵 데이터
	 *	@param width 맵의 폭
	 *	@param height 맵의 높이
	 *	@param x 가로축 위치
	 *	@param y 세로축 위치
	 *	@param pixel 맵 정보
	 *	@return void<br>
	 */
	inline void SetPixel (BYTE *image, int width, int height, int x, int y, BYTE pixel)	{ *(image + width * y + x) = pixel; }

	/**
	 *	위상학적 전역맵에서 입력되는 위치에 끝 노드를 찾는다.
	 *	@param x 가로축 위치
	 *	@param y 세로축 위치
	 *	@return true - 찾기 성공<br>
	 *	        false - 찾기 실패<br>
	 */
	bool FindEndNode (int x, int y);

	/**
	 *	위상학적 전역맵에서 입력되는 위치에 모서리 노드를 찾는다.
	 *	@param x 가로축 위치
	 *	@param y 세로축 위치
	 *	@param areaSize 입력되는 x, y 위치에서 areaSize 만큼 영역을 확장
	 *	@param angle 모서리 노드의 입각에 대한 문턱치 각도 값
	 *	@return true - 찾기 성공<br>
	 *	        false - 찾기 실패<br>
	 */
	bool FindCornerNode (int x, int y, int areaSize, int angle);

	/**
	 *	위상학적 전역맵에서 입력되는 위치에 3군대로 갈라지는 노드를 찾는다.
	 *	@param x 가로축 위치
	 *	@param y 세로축 위치
	 *	@return true - 찾기 성공<br>
	 *	        false - 찾기 실패<br>
	 */
	bool FindBranchNode (int x, int y);

	/**
	 *	위상학적 전역맵에서 3종류(끝 노드, 모서리 노드, 갈라지는 노드)의 노드들을 모드 찾는다.
	 *	@return true - 찾기 성공<br>
	 *	        false - 찾기 실패<br>
	 */
	bool FindAllNodes ();

	/**
	 *	위상학적 전역맵에서 찾은 모든 노드들에 대해 불필요한 노드들을 삭제한다.
	 *	@return true - 노드 최적화 성공<br>
	 * 	        false - 노드 최적화 실패<br>
	 */
	bool OptimizeNode ();

	/**
	 *	최적화된 노드들에 대한 연결 정보를 찾는다.
	 *	@return true - 찾기 성공<br>
	 *	        false - 찾기 실패<br>
	 */
	bool FindNodeLink ();

	/**
	 *	위상학적 전역맵에서 입력되는 위치와 연결되는 노드들과 개수를 찾는다
	 *	@param image 맵 데이터
	 *	@param width 맵의 폭
	 *	@param height 맵의 높이
	 *	@param x 가로축 위치
	 *	@param y 세로축 위치
	 *	@param nx[] 연결된 노드의 가로축 위치
	 *	@param ny[] 연결된 노드의 세로축 위치
	 *	@return true - 찾기 성공<br>
	 *	        false - 찾기 실패<br>
	 */
	int GetNumOfNodeLink (BYTE *image, int width, int height, int x,  int y, int nx[], int ny[]);

	/**
	 *	경로 생성시 입력되는 시작점과 끝점이 장애물과 겹칠 때 장애물을 겹치지 않는 재일 가까운 빈 공간의 위치를 계산한다.
	 *	@param point 찾고자 하는 위치. 입력과 동시에 결과를 얻는 위치 변수
	 *	@return true - 위치 계산 성공<br>
	 *	        false - 위치 계산 실패<br>
	 */
	bool FindNearestPoint (ObjectLocation *point);

	/**
	 *	모든 노드간 연결 정보를 이용해 시작위치에서 끝 위치까지 최단 거리를 연결하는 경로들을 계산한다.
	 *	@param ep 끝 위치
	 *	@return true - 최단 경로 계산 성공<br>
	 *	        false - 최단 계산 실패<br>
	 */
	bool SpreadToGoal (ObjectLocation &sp, ObjectLocation &ep, deque<ObjectLocation> *path);

	/**
	 *	계산된 최단 경로 위치를 정렬한다.
	 *	@param path 정렬된 최단 경로 데이터
	 *	@return void<br>
	 */
	bool TraceShortestPath (deque<ObjectLocation> *path, sTpNode *node, ObjectLocation &sp, ObjectLocation &ep);
};

#endif

