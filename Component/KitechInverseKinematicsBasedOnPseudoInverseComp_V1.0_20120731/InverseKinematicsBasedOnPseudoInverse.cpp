/*************************************************************

 file: InverseKinematicsBasedOnPseudoInverse.cpp
 author: E.C. Shin
 begin: January 31 2011
 copyright: (c) 2011 KITECH, OPRoS
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

#include "InverseKinematicsBasedOnPseudoInverse.h"

InverseKinematicsBasedOnPseudoInverse::InverseKinematicsBasedOnPseudoInverse(int maximumIteration, double epsilon)
: _maximumIteration(maximumIteration), _epsilon(epsilon)
{
	_joints = NULL;
	_fkSolver = NULL;
	_ikvSolver = NULL;
	_ikpSolver = NULL;
}

InverseKinematicsBasedOnPseudoInverse::~InverseKinematicsBasedOnPseudoInverse(void)
{
	ClearJoints();
}

int InverseKinematicsBasedOnPseudoInverse::AddJoint(int jointType, int jointAxis, double x, double y, double z)
{
	if(_joints == NULL) {
		_joints = new Chain();
	}

	if(jointType == 0) {		//	Fixed Joint
		_joints->addSegment(Segment(Joint(Joint::None), Frame(Vector(x, y, z))));
	}
	else if(jointType == 1) {	//	Revolute Joint
		if(jointAxis == 0)			_joints->addSegment(Segment(Joint(Joint::RotX), Frame(Vector(x, y, z))));
		else if(jointAxis == 1) 	_joints->addSegment(Segment(Joint(Joint::RotY), Frame(Vector(x, y, z))));
		else if(jointAxis == 2)		_joints->addSegment(Segment(Joint(Joint::RotZ), Frame(Vector(x, y, z))));
		else						return -1;
	}
	else if(jointType == 2) {	//	Translational Joint
		if(jointAxis == 0)			_joints->addSegment(Segment(Joint(Joint::TransX), Frame(Vector(x, y, z))));
		else if(jointAxis == 1) 	_joints->addSegment(Segment(Joint(Joint::TransY), Frame(Vector(x, y, z))));
		else if(jointAxis == 2) 	_joints->addSegment(Segment(Joint(Joint::TransZ), Frame(Vector(x, y, z))));
		else						return -1;
	}
	else 	return -1;

	if(_fkSolver != NULL) {
		delete _fkSolver;		_fkSolver = NULL;
	}
	_fkSolver = new ChainFkSolverPos_recursive(*_joints);

	if(_ikvSolver != NULL) {
		delete _ikvSolver;		_ikvSolver = NULL;
	}
	_ikvSolver = new ChainIkSolverVel_pinv(*_joints);

	if(_ikpSolver != NULL) {
		delete _ikpSolver;		_ikpSolver = NULL;
	}
	_ikpSolver = new ChainIkSolverPos_NR(*_joints, *_fkSolver, *_ikvSolver, _maximumIteration, _epsilon);

	return 0;
}

int InverseKinematicsBasedOnPseudoInverse::ClearJoints(void)
{
	if(_ikpSolver != NULL)		delete _ikpSolver;
	if(_ikvSolver != NULL)		delete _ikvSolver;
	if(_fkSolver != NULL)		delete _fkSolver;
	if(_joints != NULL)			delete _joints;

	return 0;
}

int InverseKinematicsBasedOnPseudoInverse::SolveForwardKinematics(vector<double> actualJointPosition, ObjectPosition &actualPosition)
{
	if(_joints == NULL || _fkSolver == NULL) {
		return -1;
	}

	if(_joints->getNrOfJoints() != actualJointPosition.size()) {
		return -1;
	}

	JntArray q = JntArray(_joints->getNrOfJoints());
	for(size_t i = 0; i < _joints->getNrOfJoints(); i++) {
		q(i) = actualJointPosition[i];
	}

	KDL::Frame position;
	if(_fkSolver->JntToCart(q, position) < 0) {
		return -1;
	}

	actualPosition.x = position.p.x();
	actualPosition.y = position.p.y();
	actualPosition.z = position.p.z();
	position.M.GetRPY(actualPosition.roll, actualPosition.pitch, actualPosition.yaw);

	return 0;
}

int InverseKinematicsBasedOnPseudoInverse::SolveInverseKinematics(vector<double> actualJointPosition, ObjectPosition desiredPosition, vector<double> &desiredJointPosition)
{
	if(_joints == NULL || _fkSolver == NULL || _ikvSolver == NULL || _ikpSolver == NULL) {
		return -1;
	}

	if(_joints->getNrOfJoints() != actualJointPosition.size()) {
		return -1;
	}

	if(_joints->getNrOfJoints() != desiredJointPosition.size()) {
		desiredJointPosition.resize(_joints->getNrOfJoints());
	}

	JntArray dq = JntArray(_joints->getNrOfJoints());
	JntArray q = JntArray(_joints->getNrOfJoints());
	for(size_t i = 0; i < _joints->getNrOfJoints(); i++) {
		q(i) = actualJointPosition[i];
	}

	KDL::Frame position;
	position.p.x(desiredPosition.x);
	position.p.y(desiredPosition.y);
	position.p.z(desiredPosition.z);
	position.M = Rotation::RPY(desiredPosition.roll, desiredPosition.pitch, desiredPosition.yaw);

	if(_ikpSolver->CartToJnt(q, position, dq) < 0) {
		return -1;
	}

	for(size_t i = 0; i < _joints->getNrOfJoints(); i++) {
		desiredJointPosition[i] = dq(i);
	}
	
	return 0;
}
