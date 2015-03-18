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
*	KITECH_TopologicalPathPlanningComp�� �̵� �κ��� ������ ���� ���� ��θ� ��ȹ�ϴ� ������Ʈ�̴�.
*	KITECH_TopologicalPathPlanningComp�� ���� ��θ� ��ȹ�ϴ� ������Ʈ�ν� ��� ��ȹ�� ���� ǥ�� �������̽��� IPathPlanningCompService�� ��ӹ޾� �����ȴ�.
*	Topological�� ������ ������ �����͸� ����Ͽ� ���� ��ġ���� ������ ��ġ���� ��θ� ��ȹ�Ѵ�.
*	���� ���̺귯���� �����Ǹ� KITECH_TopologicalGlobalMapComp�� �����Ͽ� ���ȴ�.
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
	 *	�ĸ����͸� �����ϴ� ǥ�� �Լ��̴�.
	 *	@param parameter �Ķ���� ����.
	 *	@return true - �ĸ����� ���� ����<br>
	 *			false - �Ķ���� ���� ����<br>
	 */
	virtual bool SetParameter(Property parameter);

	/**
	 *	�ĸ����͸� ��� ǥ�� �Լ��̴�.
	 *	@return parameter �Ķ���͸� ��� ���� ������ ����<br>
	 */
	virtual Property GetParameter();

	virtual int GetError();

	/**
	 *	�κ��� ���� ��ġ���� ������ ��ġ���� �ִ� �Ÿ��� ��θ� �����ϴ� ǥ�� �Լ��̴�.
	 *	@param startPosition ��� ��ġ.
	 *	@param endPoaition ���� ��ġ.
	 *	@param path ������ ��� ������.
	 *	@return true - ��� ��ȹ ����<br>
	 *	        false - ��� ��ȹ ����<br>
	 */
	virtual vector<ObjectLocation> FindPath(ObjectLocation startPosition,ObjectLocation endPosition);

private:
	/**
	 *	�Ķ���͸� �����ϴ� �����̴�.
	 *	�� ������Ʈ������ ������� �ʴ´�.
	 */
	Property parameter;

	int error;

	/**
	 *	�������� ���� ������ �����ϴ� ����
	 */
	vector<sTpNode> topologicalMap;

	/**
	 *	�ԷµǴ� �� �����Ϳ��� �ش� ��ġ�� �� ������ ��´�.
	 *	@param image �� ������
	 *	@param width ���� ��
	 *	@param height ���� ����
	 *	@param x ������ ��ġ
	 *	@param y ������ ��ġ
	 *	@return �� ����<br>
	 */
	inline BYTE GetPixel (BYTE *image, int width, int height, int x, int y)	{ return *(image + width * y + x); }

	/**
	 *	�ԷµǴ� �� �����Ϳ� �ش� ��ġ�� �� ������ �����Ѵ�.
	 *	@param image �� ������
	 *	@param width ���� ��
	 *	@param height ���� ����
	 *	@param x ������ ��ġ
	 *	@param y ������ ��ġ
	 *	@param pixel �� ����
	 *	@return void<br>
	 */
	inline void SetPixel (BYTE *image, int width, int height, int x, int y, BYTE pixel)	{ *(image + width * y + x) = pixel; }

	/**
	 *	�������� �����ʿ��� �ԷµǴ� ��ġ�� �� ��带 ã�´�.
	 *	@param x ������ ��ġ
	 *	@param y ������ ��ġ
	 *	@return true - ã�� ����<br>
	 *	        false - ã�� ����<br>
	 */
	bool FindEndNode (int x, int y);

	/**
	 *	�������� �����ʿ��� �ԷµǴ� ��ġ�� �𼭸� ��带 ã�´�.
	 *	@param x ������ ��ġ
	 *	@param y ������ ��ġ
	 *	@param areaSize �ԷµǴ� x, y ��ġ���� areaSize ��ŭ ������ Ȯ��
	 *	@param angle �𼭸� ����� �԰��� ���� ����ġ ���� ��
	 *	@return true - ã�� ����<br>
	 *	        false - ã�� ����<br>
	 */
	bool FindCornerNode (int x, int y, int areaSize, int angle);

	/**
	 *	�������� �����ʿ��� �ԷµǴ� ��ġ�� 3����� �������� ��带 ã�´�.
	 *	@param x ������ ��ġ
	 *	@param y ������ ��ġ
	 *	@return true - ã�� ����<br>
	 *	        false - ã�� ����<br>
	 */
	bool FindBranchNode (int x, int y);

	/**
	 *	�������� �����ʿ��� 3����(�� ���, �𼭸� ���, �������� ���)�� ������ ��� ã�´�.
	 *	@return true - ã�� ����<br>
	 *	        false - ã�� ����<br>
	 */
	bool FindAllNodes ();

	/**
	 *	�������� �����ʿ��� ã�� ��� ���鿡 ���� ���ʿ��� ������ �����Ѵ�.
	 *	@return true - ��� ����ȭ ����<br>
	 * 	        false - ��� ����ȭ ����<br>
	 */
	bool OptimizeNode ();

	/**
	 *	����ȭ�� ���鿡 ���� ���� ������ ã�´�.
	 *	@return true - ã�� ����<br>
	 *	        false - ã�� ����<br>
	 */
	bool FindNodeLink ();

	/**
	 *	�������� �����ʿ��� �ԷµǴ� ��ġ�� ����Ǵ� ����� ������ ã�´�
	 *	@param image �� ������
	 *	@param width ���� ��
	 *	@param height ���� ����
	 *	@param x ������ ��ġ
	 *	@param y ������ ��ġ
	 *	@param nx[] ����� ����� ������ ��ġ
	 *	@param ny[] ����� ����� ������ ��ġ
	 *	@return true - ã�� ����<br>
	 *	        false - ã�� ����<br>
	 */
	int GetNumOfNodeLink (BYTE *image, int width, int height, int x,  int y, int nx[], int ny[]);

	/**
	 *	��� ������ �ԷµǴ� �������� ������ ��ֹ��� ��ĥ �� ��ֹ��� ��ġ�� �ʴ� ���� ����� �� ������ ��ġ�� ����Ѵ�.
	 *	@param point ã���� �ϴ� ��ġ. �Է°� ���ÿ� ����� ��� ��ġ ����
	 *	@return true - ��ġ ��� ����<br>
	 *	        false - ��ġ ��� ����<br>
	 */
	bool FindNearestPoint (ObjectLocation *point);

	/**
	 *	��� ��尣 ���� ������ �̿��� ������ġ���� �� ��ġ���� �ִ� �Ÿ��� �����ϴ� ��ε��� ����Ѵ�.
	 *	@param ep �� ��ġ
	 *	@return true - �ִ� ��� ��� ����<br>
	 *	        false - �ִ� ��� ����<br>
	 */
	bool SpreadToGoal (ObjectLocation &sp, ObjectLocation &ep, deque<ObjectLocation> *path);

	/**
	 *	���� �ִ� ��� ��ġ�� �����Ѵ�.
	 *	@param path ���ĵ� �ִ� ��� ������
	 *	@return void<br>
	 */
	bool TraceShortestPath (deque<ObjectLocation> *path, sTpNode *node, ObjectLocation &sp, ObjectLocation &ep);
};

#endif

