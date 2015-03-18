/*************************************************************

file: KITECH_TopologicalPathPlanningComp.cpp
author: E.C. Shin
begin: January 30 2010
copyright: (c) 2010 KITECH, OPRoS
email: unchol@kitech.re.kr

***************************************************************

OPRoS source code is provided under a dual license mode:
LGPL and OPRoS individually.

LGPL: You can redistribute it and/or modify it under the terms
of the Less GNU General Public License as published by the Free
Software Foundation, either version 3 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
Less GNU General Public License for more details. 

You should have received a copy of the Less GNU General Public
License along with this program. If not, see 
<http://www.gnu.org/licenses/>.

OPRoS individual license: An individual license is a license for
modifying the source code of OPRoS and distiributing it in a
closed format for commercial purposes.

If you are interested in this option, please see 
<http://www.opros.or.kr>.

This license policy may be changed without prior notice.

***************************************************************/
/*
 *  Generated sources by OPRoS Component Generator (OCG V2)
 *  
 */
#include <Component.h>
#include <InputDataPort.h>
#include <OutputDataPort.h>
#include <InputEventPort.h>
#include <OutputEventPort.h>
#include <OPRoSTypes.h>
#include <EventData.h>

#include "PathPlanningServiceProvided.h"
#include "KITECH_TopologicalPathPlanningComp.h"
#include "OprosPrintMessage.h"


//
// constructor declaration
//
KITECH_TopologicalPathPlanningComp::KITECH_TopologicalPathPlanningComp()
{
	error = 0;
	portSetup();
}

//
// constructor declaration (with name)
//
KITECH_TopologicalPathPlanningComp::KITECH_TopologicalPathPlanningComp(const std::string &name):Component(name)
{
	error = 0;
	portSetup();
}

//
// destructor declaration
//
KITECH_TopologicalPathPlanningComp::~KITECH_TopologicalPathPlanningComp()
{
}

void KITECH_TopologicalPathPlanningComp::portSetup()
{
	// provided service port setup
	ProvidedServicePort *pa1 = new PathPlanningServiceProvided(this);
	addPort("PathPlanningService",pa1);

	// required service port setup
	ptrGlobalMapService = new GlobalMapServiceRequired();
	addPort ("GlobalMapService", ptrGlobalMapService);
}

// Call back Declaration
ReturnType KITECH_TopologicalPathPlanningComp::onInitialize()
{
	PrintMessage ("KITECH_TopologicalPathPlanningComp::onInitialize()\n");

	parameter.SetProperty(getPropertyMap());

	topologicalMap.clear ();

	error = 0;

	return OPROS_SUCCESS;
}

ReturnType KITECH_TopologicalPathPlanningComp::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType KITECH_TopologicalPathPlanningComp::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KITECH_TopologicalPathPlanningComp::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KITECH_TopologicalPathPlanningComp::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KITECH_TopologicalPathPlanningComp::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KITECH_TopologicalPathPlanningComp::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KITECH_TopologicalPathPlanningComp::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KITECH_TopologicalPathPlanningComp::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType KITECH_TopologicalPathPlanningComp::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType KITECH_TopologicalPathPlanningComp::onPeriodChanged()
{
	// user code here
	return OPROS_SUCCESS;
}

bool KITECH_TopologicalPathPlanningComp::SetParameter(Property parameter)
{
	this->parameter = parameter;
	return true;
}

Property KITECH_TopologicalPathPlanningComp::GetParameter()
{
	error = 0;
	return parameter;
}

int KITECH_TopologicalPathPlanningComp::GetError()
{
	return error;
}

/*
	<환경 조건>
	1. Tetra 로봇 플렛폼
	2. Intel Core2Duo 2.1GHz
	3. 950 x 850 크기의 map

	<실험>
	1. FindPath() 수행 속도 측정
		0ms ~ 16ms
*/
vector<ObjectLocation> KITECH_TopologicalPathPlanningComp::FindPath(ObjectLocation startPosition,ObjectLocation endPosition)
{
	deque<ObjectLocation> path;
	vector<ObjectLocation> path2;

	path.clear ();
	path2.clear ();

	error = 0;

	if (topologicalMap.size () == 0) {
		bool ret = FindAllNodes ();				/* 세선화된 이미지에서 모든 노드들을 찾아 저장 */
		ret &= OptimizeNode ();					/* 노드들을 그룹핑해서 불필요한 노드는 제거 */
		ret &= FindNodeLink ();					/* 최적화된 노드들의 link 정보를 찾아 저장 */
		if (ret == false) {
			error = -1;
			return path2;
		}
	}

	ObjectLocation sp(startPosition);
	ObjectLocation ep(endPosition);
	if (ptrGlobalMapService != NULL) {
		if (ptrGlobalMapService->IsIn(sp.x, sp.y)) {
			if (10 <= ptrGlobalMapService->GetPixel(sp.x, sp.y)) {
				if (!FindNearestPoint(&sp)) {
					error = -1;
					return path2;	
				}
			}
		}
		if (ptrGlobalMapService->IsIn(ep.x, ep.y)) {
			if (10 <= ptrGlobalMapService->GetPixel(ep.x, ep.y)) {
				if (!FindNearestPoint(&ep)) {
					error = -1;
					return path2;
				}
			}
		}
	}
	else {
		error = -1;
		return path2;
	}

	vector<sTpNode>::iterator i1;
	for (i1 = topologicalMap.begin (); i1 != topologicalMap.end (); i1++){
		i1->timeElasped = 0;
		i1->cameFrom = NULL;
	}

	if (!SpreadToGoal (sp, ep, &path)) {
		path2.clear();
		error = -1;
		return path2;
	}
	
	int size = (int)path.size ();
	for (int i = 0; i < size; i++) {
		path2.push_back (*path.begin());
		path.pop_front ();
	}

	return path2;;
}

bool KITECH_TopologicalPathPlanningComp::FindEndNode (int x, int y)
{
	const int matrix_size = 3;	
	BYTE a[matrix_size*matrix_size];
	int pixel_count = 0;
	
	if (ptrGlobalMapService == NULL) return false;

	int mapWidth = ptrGlobalMapService->GetMapWidth();
	int mapHeight = ptrGlobalMapService->GetMapHeight();

	for (int j = 0; j < matrix_size; j++) {
		for (int i = 0; i < matrix_size; i++) {
			int x1 = x - (matrix_size / 2) + i;
			int y1 = y - (matrix_size / 2) + j;
			if ((0 <= x1) && (x1 < mapWidth) && (0 <= y1) && (y1 < mapHeight)) {
				BYTE pixel = ptrGlobalMapService->GetPixelCell(x1, y1);
				SetPixel (a, matrix_size, matrix_size, i, j, pixel);
			}
			if (0 < GetPixel(a, matrix_size, matrix_size, i, j)) {
				if ((j == 0) || (j == matrix_size-1) || (i == 0) || (i == matrix_size-1)) {
					pixel_count++;
				}
			}
		}
	}

	if (pixel_count == 1) {
		return true;
	}

	return false;
}

bool KITECH_TopologicalPathPlanningComp::FindCornerNode (int x, int y, int areaSize, int angle)
{
	if (ptrGlobalMapService == NULL) return false;

	int matrix_size = areaSize;	
	BYTE *a = new BYTE [matrix_size*matrix_size];
	//int pixel_count = 0;
	vector<ObjectLocation> p;

	int mapWidth = ptrGlobalMapService->GetMapWidth();
	int mapHeight = ptrGlobalMapService->GetMapHeight();

	p.clear();
	p.push_back (ObjectLocation (x, y, 0));
	for (int j = 0; j < matrix_size; j++) {
		for (int i = 0; i < matrix_size; i++) {
			int x1 = x - (matrix_size/2) + i;
			int y1 = y - (matrix_size/2) + j;
			if ((0 <= x1) && (x1 < mapWidth) && (0 <= y1) && (y1 < mapHeight)) {
				BYTE pixel = ptrGlobalMapService->GetPixelCell(x1, y1);
				SetPixel (a, matrix_size, matrix_size, i, j, pixel);
			}
			if (GetPixel(a, matrix_size, matrix_size, i, j) > 0) {
				if ((j == 0) || (j == matrix_size-1) || (i == 0) || (i == matrix_size-1)) {
					p.push_back (ObjectLocation (x1, y1, 0));
				}
			}
		}
	}
	delete [] a;

	if (p.size () != 3){
		return false;
	}

	vector<ObjectLocation>::iterator it = p.begin ();
	ObjectLocation o(0,0,0), p1(0,0,0), p2(0,0,0);
	double distOP1, distOP2, distP1P2;
	double _angle;

	o.x = it->x;
	o.y = it->y;
	p1.x = (it+1)->x;
	p1.y = (it+1)->y;
	p2.x = (it+2)->x;
	p2.y = (it+2)->y;

	distOP1 = o.Distance (p1);
	distOP2 = o.Distance (p2);
	distP1P2 = p1.Distance (p2);

	_angle = acos ( ((distOP1*distOP1)+(distOP2*distOP2)-(distP1P2*distP1P2)) / (2*distOP1*distOP2) );
	_angle = RAD2DEG(_angle);

	if (_angle < angle){
		return true;
	}

	return false;
}

bool KITECH_TopologicalPathPlanningComp::FindBranchNode (int x, int y)
{
	const int matrix_size = 3;	
	BYTE a[matrix_size*matrix_size];
	int pixel_count = 0;
	int i, j;

	if (ptrGlobalMapService == NULL) return false;

	int mapWidth = ptrGlobalMapService->GetMapWidth();
	int mapHeight = ptrGlobalMapService->GetMapHeight();

	for (j = 0; j < matrix_size; j++) {
		for (i = 0; i < matrix_size; i++) {
			int x1 = x - (matrix_size/2) + i;
			int y1 = y - (matrix_size/2) + j;
			if ((0 <= x1) && (x1 < mapWidth) && (0 <= y1) && (y1 < mapHeight)) {
				BYTE pixel = ptrGlobalMapService->GetPixelCell(x1, y1);
				SetPixel (a, matrix_size, matrix_size, i, j, pixel);
			}
		}
	}

	for (i = 0; i < matrix_size-1; i++){	// x축
		if (GetPixel (a, matrix_size, matrix_size, i, 0) == 4){
			pixel_count++;
		}
		if((*(a+0*matrix_size+i)) && (*(a+0*matrix_size+i+1))){
			return false;
		}
	}
	for (j = 0; j < matrix_size-1; j++){	// y축 
		if (*(a+j*matrix_size+(matrix_size-1))){
			pixel_count++;
		}
		if((*(a+j*matrix_size+(matrix_size-1))) && (*(a+(j+1)*matrix_size+(matrix_size-1)))){
			return false;
		}
	}
	for (i = matrix_size-1; i > 0; i--){	// x축 
		if (*(a+(matrix_size-1)*matrix_size+i)){
			pixel_count++;
		}
		if((*(a+(matrix_size-1)*matrix_size+i)) && (*(a+(matrix_size-1)*matrix_size+i-1))){
			return false;
		}
	}
	for (j = matrix_size-1; j > 0; j--){	// y축 
		if (*(a+j*matrix_size+0)){
			pixel_count++;
		}
		if((*(a+j*matrix_size+0)) && (*(a+(j-1)*matrix_size+0))){
			return false;
		}
	}

	if (pixel_count < 3){
		return false;
	}

	return true;
}

bool KITECH_TopologicalPathPlanningComp::FindAllNodes ()
{
	int x, y;

	const int conerNodeAreaSize = 5;	// 15
	const int connerNodeAngle = 160;	// 168

	//topologicalMap.clear ();
	if (ptrGlobalMapService == NULL) return false;

	int mapWidth = ptrGlobalMapService->GetMapWidth();
	int mapHeight = ptrGlobalMapService->GetMapHeight();
	for (y = 1; y < (mapHeight-1); y++) {
		for (x = 1; x < (mapWidth-1); x++) {
			BYTE pixel = ptrGlobalMapService->GetPixelCell(x, y);
			if (pixel == 4) {
				if (FindEndNode (x, y)) {
					topologicalMap.push_back(sTpNode (x, y));
				}
				else if (FindCornerNode (x, y, conerNodeAreaSize, connerNodeAngle)) {
					topologicalMap.push_back(sTpNode (x, y));
				}
				else if (FindBranchNode (x, y)) {
					topologicalMap.push_back(sTpNode (x, y));
				}
				else{
				}
			}
		}
	}

	return true;
}

bool KITECH_TopologicalPathPlanningComp::OptimizeNode ()
{
	if (ptrGlobalMapService == NULL) return false;

	vector<sTpNode>::iterator i1, i2;
	vector<sTpNode> m1;
	vector<sTpNode> _Node;

	const int maxDist = 10;
	bool skip = false;

	m1.clear ();
	for (i1 = topologicalMap.begin (); i1 != topologicalMap.end (); i1++) {
		skip = false;
		for (i2 = m1.begin (); i2 != m1.end (); i2++) {							/* 자신의 노드는 skip */
			if ((i1->x == i2->x) && (i1->y == i2->y)) {
				skip = true;
				break;
			}
		}
		if (skip == true) { continue; }

		m1.clear ();															/* 그룹 노드 백터(m1)를 클리어 */
		m1.push_back (sTpNode (i1->x, i1->y));									/* 기준이 되는 노드 m1에 저장 */
		for (i2 = topologicalMap.begin (); i2 != topologicalMap.end (); i2++){	/* 기준 노드 주위의 노드를 모두 찾아서 m1에 저장 */
			if (i1 != i2){
				if (Distance (i1->x, i1->y, i2->x, i2->y) < maxDist){
					m1.push_back (sTpNode (i2->x, i2->y));
				}
			}
		}

		if (m1.size () > 1){
			double sumX = 0, sumY = 0;
			struct sTpNode tempNode (0, 0);

			for (i2 = m1.begin (); i2 != m1.end (); i2++){						/* 그룹 노드의 모든 좌표를 sum */
				sumX += i2->x;
				sumY += i2->y;
			}
			sumX /= m1.size ();													/* 그룹 노드의 중심 좌표를 계산 */
			sumY /= m1.size ();
#if 0
			int minDist = maxDist;
			for (i2 = m1.begin (); i2 != m1.end (); i2++){
				int dist = Distance (sumX, sumY, i2->x, i2->y);
				if (minDist > dist){
					minDist = dist;
					tempNode.x = i2->x;
					tempNode.y = i2->y;
					tempNode.type = i2->type;
				}
			}
			_Node.push_back (tempNode);
#else
			tempNode.x = (int)(sumX+0.5);
			tempNode.y = (int)(sumY+0.5);
			_Node.push_back (tempNode);											/* 그룹 노드의 대표되는 노드를 저장 */
#endif
		}
		else{
			_Node.push_back (sTpNode (m1.begin()->x, m1.begin()->y));
		}
	}

	topologicalMap.clear ();
	for (i1 = _Node.begin (); i1 != _Node.end (); i1++) {						/* 최적화 된 노드들에서 중복되는 노드는 제거 */

		bool equality = false;

		for (i2 = topologicalMap.begin (); i2 != topologicalMap.end (); i2++) {
			if ((i1->x == i2->x) && (i1->y == i2->y)) {
				equality = true;
				break;
			}
		}

		if (equality == false){
			double x = i1->x * ptrGlobalMapService->GetMapCellWidth();
			double y = i1->y * ptrGlobalMapService->GetMapCellWidth();
			topologicalMap.push_back (sTpNode (x, y));
		}
	}

	return true;
}

int KITECH_TopologicalPathPlanningComp::GetNumOfNodeLink (BYTE *image, int width, int height, int x,  int y, int nx[], int ny[])
{
	const int matrixSize = 3;	
	BYTE matrix[matrixSize*matrixSize];
	int matrixIndex = 0;
	int i, j;
	int x1, y1;

	for (j = 0; j < matrixSize; j++){
		for (i = 0; i < matrixSize; i++){
			x1 = x - (matrixSize/2) + i;
			y1 = y - (matrixSize/2) + j;
			if ((x1 >= 0) && (x1 < width) && (y1 >= 0) && (y1 < height)){
				SetPixel (matrix, matrixSize, matrixSize, i, j, GetPixel (image, width, height, x1, y1));
			}
			if (GetPixel (matrix, matrixSize, matrixSize, i, j) == 4){
				if ((j == 0) || (j == matrixSize-1) || (i == 0) || (i == matrixSize-1)){
					nx[matrixIndex] = x1;
					ny[matrixIndex] = y1;
					matrixIndex++;
				}
			}
		}
	}

	return matrixIndex;
}

bool KITECH_TopologicalPathPlanningComp::FindNodeLink ()
{
	if (ptrGlobalMapService == NULL) return false;

	int mapWidth = ptrGlobalMapService->GetMapWidth();
	int mapHeight = ptrGlobalMapService->GetMapHeight();

	BYTE *imgBuff = new BYTE [mapWidth*mapHeight];
	vector<sTpNode>::iterator i1, i2, temp;

	for (i1 = topologicalMap.begin (); i1 != topologicalMap.end (); i1++) {
		int nx[10], ny[10];
		i1->route.clear ();

		for (int y = 0; y < mapHeight; y++)
			for (int x = 0; x < mapWidth; x++)
				SetPixel(imgBuff, mapWidth, mapHeight, x, y, ptrGlobalMapService->GetPixelCell(x,y));

		int x = ptrGlobalMapService->M2CUx(i1->x);
		int y = ptrGlobalMapService->M2CUy(i1->y);
		int nodeLinkNum = GetNumOfNodeLink (imgBuff, mapWidth, mapHeight, x, y, nx, ny);

		if (nodeLinkNum == 0) continue;
		SetPixel (imgBuff, mapWidth, mapHeight, x, y, 0);

		for (int i = 0; i < nodeLinkNum; i++) {
			int x1 = nx[i];
			int y1 = ny[i];
			bool found = false;
			SetPixel (imgBuff, mapWidth, mapHeight, x1, y1, 0);	/* 기준점 위치(x1, y1) 픽셀 값을 지움 */

			while(1){
				int x2, y2;

				if (x1 < 1 || (mapWidth-1) <= x1 || y1 < 1 || (mapHeight-1) <= y1) break;

				if (GetPixel (imgBuff, mapWidth, mapHeight, x1, y1-1) == 4) { found = true; x2 = x1; y2 = y1 - 1; }
				else if (GetPixel (imgBuff, mapWidth, mapHeight, x1+1, y1) == 4) { found = true; x2 = x1 + 1; y2 = y1; }
				else if (GetPixel (imgBuff, mapWidth, mapHeight, x1, y1+1) == 4) {	found = true; x2 = x1; y2 = y1 + 1; }
				else if (GetPixel (imgBuff, mapWidth, mapHeight, x1-1, y1) == 4) { found = true; x2 = x1 - 1; y2 = y1; }
				else if (GetPixel (imgBuff, mapWidth, mapHeight, x1-1, y1-1) == 4) { found = true; x2 = x1 - 1; y2 = y1 - 1; }
				else if (GetPixel (imgBuff, mapWidth, mapHeight, x1+1, y1-1) == 4) { found = true; x2 = x1 + 1; y2 = y1 - 1; }
				else if (GetPixel (imgBuff, mapWidth, mapHeight, x1+1, y1+1) == 4) { found = true; x2 = x1 + 1; y2 = y1 + 1; }
				else if (GetPixel (imgBuff, mapWidth, mapHeight, x1-1, y1+1) == 4) { found = true; x2 = x1 - 1; y2 = y1 + 1; }
				else { found = false; }

				if (!found){ break; }

				found = false;
				for (i2 = topologicalMap.begin (); i2 != topologicalMap.end (); i2++) {
					int x = ptrGlobalMapService->M2CUx (i2->x);
					int y = ptrGlobalMapService->M2CUy (i2->y);
					if (((x2-1) <= x && (x2+1) >= x)  
						&& ((y2-1) <= y && (y2+1 >= y))){
							temp = i2;
							found = true;
							break;
					}
				}

				if (found){
					struct sTpRoute route;
					route.distance = Distance (i1->x, i1->y, temp->x, temp->y);
					route.connectedNode = &(*temp);
					i1->route.push_back (route);
					break;
				}
				else {
					SetPixel (imgBuff, mapWidth, mapHeight, x2, y2, 0);	/* 찾은 이웃집(x2, y2) 픽셀을 지움 */
				}
				x1 = x2;
				y1 = y2;
			}
		}
	}

	delete [] imgBuff;
	return true;
}

bool KITECH_TopologicalPathPlanningComp::FindNearestPoint (ObjectLocation *point)
{
	if (ptrGlobalMapService == NULL) return false;

	int i, j;
	for (i=0; i<100; i++) {
		for (j=0; j<16; j++) {
			double r  = i*ptrGlobalMapService->GetMapCellWidth();
			double th = DEG2RAD (rand ());
			double dx = r * cos (th);
			double dy = r * sin (th);
			if (ptrGlobalMapService->IsIn(point->x+dx, point->y+dy)) {
				if (ptrGlobalMapService->GetPixel(point->x+dx, point->y+dy) < 1) {
					point->x += dx;
					point->y += dy;
					return true;
				}
			}
		}
	}
	return false;
}

bool KITECH_TopologicalPathPlanningComp::SpreadToGoal (ObjectLocation &sp, ObjectLocation &ep, deque<ObjectLocation> *path)
{
	if (ptrGlobalMapService == NULL) return false;

	vector<sTpNode>::iterator i2;
	sTpNode *startNode = NULL;
	sTpNode *endNode = NULL;
	double maxDist = 1000;
	{
		for (i2 = topologicalMap.begin (); i2 != topologicalMap.end (); i2++) {
			ObjectLocation p(i2->x, i2->y, 0);
			double dist = Distance (sp.x, sp.y, p.x, p.y);
			bool isIn = ptrGlobalMapService->IsIntersect (sp.x, sp.y, p.x, p.y, 10);
			if ((dist < maxDist) && !isIn) {
				maxDist = dist;
				startNode = &(*i2);
			}
		}

		maxDist = 1000;
		for (i2 = topologicalMap.begin (); i2 != topologicalMap.end (); i2++) {
			ObjectLocation p(i2->x, i2->y, 0);
			double dist = Distance (ep.x, ep.y, p.x, p.y);
			bool isIn = ptrGlobalMapService->IsIntersect (ep.x, ep.y, p.x, p.y, 10);
			if ((dist < maxDist) && !isIn) {
				maxDist = dist;
				endNode = &(*i2);
			}
		}

		if (startNode == NULL || endNode == NULL) return false;
	}

	int t = 1;
	bool findGoal = false;
	vector<sTpNode *> m1, m2;

	startNode->timeElasped = t;
	m1.push_back (startNode);

	while (!m1.empty ()) {
		m2.reserve (100000);

		vector<sTpNode *>::iterator _mi;
		for (_mi = m1.begin (); _mi != m1.end(); _mi++) {
			if (t < (*_mi)->timeElasped) {
				m2.push_back (*_mi);
			}
			else {
				vector<sTpRoute>::iterator ir;
				for (ir=(*_mi)->route.begin(); ir!=(*_mi)->route.end(); ir++) {
					int timeElasped = t + 1 + (int)(ir->distance*40);
					if (ir->connectedNode->timeElasped == 0) {
						ir->connectedNode->timeElasped = timeElasped;
						ir->connectedNode->cameFrom = *_mi;
						m2.push_back (ir->connectedNode);
					}
					else if (ir->connectedNode->timeElasped > timeElasped) {
						ir->connectedNode->timeElasped = timeElasped;
						ir->connectedNode->cameFrom = *_mi;
					}
					else {

					}
				}
				if ((fabs((*_mi)->x-endNode->x) < 0.001) && (fabs((*_mi)->y-endNode->y) < 0.001)) { findGoal = true; break;}
			}
		}
		m1.clear ();
		m1.swap (m2);
		t += 1;

		if (findGoal) {
			m1.clear ();
			m2.clear ();
			TraceShortestPath (path, endNode, sp, ep);
			return true;
		}
	}

	return false;
}

bool KITECH_TopologicalPathPlanningComp::TraceShortestPath (deque<ObjectLocation> *path, sTpNode *node, ObjectLocation &sp, ObjectLocation &ep)
{
	if (ptrGlobalMapService == NULL) return false;

	sTpNode *cp = node;
	while (cp) {
		ObjectLocation p (cp->x, cp->y, 0.);
		path->push_back (p);
		cp = cp->cameFrom;
	}

	reverse (path->begin (), path->end ());

	// 불필요한 경로 삭제
	//path->push_front (sp);
	path->push_back (ep);
	deque<ObjectLocation> tempPath;
	tempPath.push_front (ep);

	int i;
	for (i = (int)path->size()-1; i > 1; i--) {
		ObjectLocation p = path->at(i);
		if (!ptrGlobalMapService->IsIntersect(sp.x, sp.y, p.x, p.y, 10)) {
			break;
		}
		tempPath.push_front (path->at(i-1));
	}

	for (i = 0; i < (int)tempPath.size(); i++) {
		ObjectLocation p = tempPath.at(i);
		if (!ptrGlobalMapService->IsIntersect(ep.x, ep.y, p.x, p.y, 10)) {
			int size = (int)tempPath.size();
			for (int j = i+1; j < size; j++) {
				tempPath.pop_back ();
			}
			break;
		}
	}

	tempPath.push_front (sp);
	tempPath.push_back (ep);

	path->swap(tempPath);

	return true;
}


#ifdef MAKE_COMPONENT_DLL
#ifdef WIN32
extern "C"
{
__declspec(dllexport) Component *getComponent();
__declspec(dllexport) void releaseComponent(Component *pcom);
}

Component *getComponent()
{
	return new KITECH_TopologicalPathPlanningComp();
}

void releaseComponent(Component *com)
{
	delete com;
}
#else
extern "C"
{
	Component *getComponent();
	void releaseComponent(IComponent *com);
}
Component *getComponent()
{
	return new KITECH_TopologicalPathPlanningComp();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

