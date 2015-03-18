#pragma once

#include "ApiTypes.h"
#include "Actuator.h"

#undef min
#undef max

#include <kdl/chain.hpp>
#include <kdl/chainfksolver.hpp>
#include <kdl/chainfksolverpos_recursive.hpp>
#include <kdl/chainiksolvervel_pinv.hpp>
#include <kdl/chainiksolverpos_nr.hpp>
#include <kdl/frames_io.hpp>
#include <vector>

using namespace KDL;
using namespace std;

class InverseKinematicsBasedOnPseudoInverse
{
public:
	InverseKinematicsBasedOnPseudoInverse(int maximumIteration = 100, double epsilon = 0.00001);
	virtual ~InverseKinematicsBasedOnPseudoInverse(void);

private:
	Chain *_joints;
	ChainFkSolverPos_recursive *_fkSolver;
	ChainIkSolverVel_pinv *_ikvSolver;
	ChainIkSolverPos_NR *_ikpSolver;

	double _epsilon;
	int _maximumIteration;

public:
	int AddJoint(int jointType, int jointAxis, double x, double y, double z);
	int ClearJoints(void);

	int SolveForwardKinematics(vector<double> actualJointPosition, ObjectPosition &actualPosition);
	int SolveInverseKinematics(vector<double> actualJointPosition, ObjectPosition desiredPosition, vector<double> &desiredJointPosition);
};

