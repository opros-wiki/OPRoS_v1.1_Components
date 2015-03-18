//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#include <evolution/core/math/Box.hpp>
#include <evolution/core/math/ClassMixture.hpp>
#include <evolution/core/math/ClassMixtureCache.hpp>
#include <evolution/core/math/Clustering.hpp>
#include <evolution/core/math/Defuzzy.hpp>
#include <evolution/core/math/EulerAngles.hpp>
#include <evolution/core/math/FuzzyConstants.hpp>
#include <evolution/core/math/FuzzyMembership.hpp>
#include <evolution/core/math/FuzzyOperators.hpp>
#include <evolution/core/math/Gaussian.hpp>
#include <evolution/core/math/GaussianMixture.hpp>
#include <evolution/core/math/KalmanFilter.hpp>
#include <evolution/core/math/Kinematics.hpp>
#include <evolution/core/math/Line.hpp>
#include <evolution/core/math/MathUtils.hpp>
#include <evolution/core/math/Matrix.hpp>
#include <evolution/core/math/Matrix2.hpp>
#include <evolution/core/math/Matrix3.hpp>
#include <evolution/core/math/MultiValue.hpp>
#include <evolution/core/math/MultiValueFunctions.hpp>
#include <evolution/core/math/NewtonRootFinder.hpp>
#include <evolution/core/math/ParseUtils.hpp>
#include <evolution/core/math/PointSet.hpp>
#include <evolution/core/math/Pose2D.hpp>
#include <evolution/core/math/Pose2DT.hpp>
#include <evolution/core/math/Pose3D.hpp>
#include <evolution/core/math/PoseCov2D.hpp>
#include <evolution/core/math/PoseCov2DT.hpp>
#include <evolution/core/math/SphericalPoint.hpp>
#include <evolution/core/math/Triangle.hpp>
#include <evolution/core/math/Vector.hpp>
#include <evolution/core/math/Vector2.hpp>
#include <evolution/core/math/Vector3.hpp>
