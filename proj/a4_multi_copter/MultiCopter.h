#ifndef __MultiCopter_h__
#define __MultiCopter_h__
#include "Common.h"
#include "RigidBody.h"

//////////////////////////////////////////////////////////////////////////
////MultiCopter simulator
template<int d> class MultiCopter
{using VectorD=Vector<real,d>;using VectorDi=Vector<int,d>;using MatrixD=Matrix<real,d>;
using Vector2=Vector<real,2>;using Matrix2=Matrix<real,2>;
public:
	RigidBody<d> rigid_body;
	Array<VectorD> rotor_pos;
	Array<VectorD> rotor_dir;
	int thrust_flag;
	Array<VectorD> thrust_vec;

	// Parameters
	real mass;
	VectorD inertia;
	real arm_length;
	real lambda;
	real g;
	real t;

	virtual void Initialize(const int flag)
	{
		thrust_flag = flag;
		mass = 0.068;
		inertia(0) = 1e-3 * 0.0686;
		inertia(1) = 1e-3 * 0.0920;
		inertia(2) = 1e-3 * 0.1366;
		arm_length = 0.0624;
		lambda = 0.0024;
		g = 9.81;
		t = 0.0;
		if (flag == 1)
			rigid_body.omega(2) = 0.2;// Model the spin around yaw.
	}

	const VectorD RotationToRollPitchYaw(const MatrixD& R)
	{
		// The three angles are computed based on the slides here:
		// http://www.princeton.edu/~stengel/MAE331Lecture9.pdf, page 3.
		double roll, pitch, yaw;
		const VectorD XI = VectorD::UnitX();
		const VectorD YI = VectorD::UnitY();
		const VectorD ZI = VectorD::UnitZ();
		const VectorD XB = R.col(0), YB = R.col(1), ZB = R.col(2);
		// Let's first rotate along XB to compute Y2.
		const VectorD Y2 = ZI.cross(XB).normalized();
		double cos_roll = Y2.dot(YB);
		// Clamp cosRoll (if necessary).
		if (cos_roll > 1.0) cos_roll = 1.0;
		if (cos_roll < -1.0) cos_roll = -1.0;
		roll = acos(cos_roll);
		// Check to see whether we need to swap the sign of roll.
		if (Y2.dot(ZB) > 0.0) roll = -roll;

		// Next let's rotate along Y2 so that X1 falls into XOY plane.
		const VectorD X1 = Y2.cross(ZI).normalized();
		double cos_pitch = X1.dot(XB);
		// Clamp cosPitch.
		if (cos_pitch > 1.0) cos_pitch = 1.0;
		if (cos_pitch < -1.0) cos_pitch = -1.0;
		pitch = acos(cos_pitch);
		// Check to see whether we need to swap the sign of pitch.
		if (XB(2) > 0.0) pitch = -pitch;

		// We finally need to rotate along ZI to compute yaw.
		double cos_yaw = X1.dot(XI);
		// Clamp cosYaw.
		if (cos_yaw > 1.0) cos_yaw = 1.0;
		if (cos_yaw < -1.0) cos_yaw = -1.0;
		yaw = acos(cos_yaw);
		if (X1(1) < 0) yaw = -yaw;
		return VectorD(roll, pitch, yaw);
		// Sanity check:
		// const Matrix3 R1 = AngleAxis(yaw, Vector3::UnitZ())
		//  * AngleAxis(pitch, Vector3::UnitY())
		//  * AngleAxis(roll, Vector3::UnitX()).matrix();
		// const Matrix3 R2 = AngleAxis(roll, XB)
		//   * AngleAxis(pitch, Y2)
		//   * AngleAxis(yaw, ZI).matrix();
		// std::cout << "Diff between R and R1 = " << (R - R1).norm() << std::endl;
		// std::cout << "Diff between R and R2 = " << (R - R2).norm() << std::endl;
	}

	void Add_Rotor(const Vector3& pos,const Vector3& dir)
	{
		rotor_pos.push_back(pos);
		rotor_dir.push_back(dir);
		thrust_vec.push_back(VectorD::Zero());
	}

	Vector3 World_Coord(const Vector3& local_pos)
	{
		return rigid_body.LocalPointToWorldPoint(local_pos);
	}

	// Compute Euler rates from attitude and angular rate.
	// Reference:
	const VectorD AngularRateToEulerRate(const VectorD& rpy, const MatrixD& R, const VectorD& omega)
	{
		// Reference:
		// http://www.princeton.edu/~stengel/MAE331Lecture9.pdf.
		const double roll = rpy(0), pitch = rpy(1), yaw = rpy(2);
		const double s_roll = sin(roll), c_roll = cos(roll),
			s_pitch = sin(pitch), c_pitch = cos(pitch), t_pitch = tan(pitch);
		return (MatrixD() << 1, s_roll * t_pitch, c_roll * t_pitch,
			0, c_roll, -s_roll,
			0, s_roll / c_pitch, c_roll / c_pitch).finished() * (R.transpose() * omega);
	}

	const real Clamp(const real val, const real min_val, const real max_val)
	{
		real new_val = val;
		if (new_val > max_val) new_val = max_val;
		if (new_val < min_val) new_val = min_val;
		return new_val;
	}

	virtual void Advance(const real dt, const VectorD& target)
	{
		Array<real> thrusts(4, 0.0);
		real mg = mass * g;
		if (thrust_flag == 0)
		{
			// For simulation.
			real baseline = mg * (1.0 + 0.01 * std::cos(t)) / 4.0;
			thrusts[0] = baseline + 0.001 * std::sin(t);
			thrusts[1] = baseline - 0.001 * std::sin(t);
			thrusts[2] = baseline - 0.001 * std::sin(t);
			thrusts[3] = baseline + 0.001 * std::sin(t);
		}
		else
		{
			// For control.
			// Sensing and noises.
			const VectorD p = rigid_body.position + VectorD::Random().cwiseProduct(VectorD(0.02, 0.02, 0.01));
			const real z_rate = rigid_body.velocity.z();
			const VectorD v = rigid_body.WorldVectorToLocalVector(rigid_body.velocity);
			const VectorD rpy = RotationToRollPitchYaw(rigid_body.R) + VectorD::Random().cwiseProduct(VectorD(0.05, 0.05, 0.1));
			const VectorD rpy_rate = AngularRateToEulerRate(rpy, rigid_body.R, rigid_body.omega);

			// Altitude control.
			const real z_ref = target.z();
			const real z = p.z();
			// Clamp total_thrust between 0.5mg and 1.5mg.
			const real min_thrust = 0.5 * mass * g, max_thrust = 1.5 * mass * g;
			real total_thrust = Clamp(AltitudeController(mass * g, z_ref, z, z_rate), min_thrust, max_thrust);

			// Yaw control.
			const real yaw = rpy(2);
			const real yaw_rate = rpy_rate(2);
			const real tau_yaw = YawController(0.0, yaw, yaw_rate);

			// Xy control.
			const Vector2 pitch_roll_cmd = XyController(yaw, target.head(2), p.head(2), v.head(2));
			// A simple PD controller.
			const real pitch_ref = pitch_roll_cmd(0), roll_ref = pitch_roll_cmd(1);
			const real tau_pitch = 0.013 * (pitch_ref - rpy(1)) - 0.002 * rpy_rate(1);
			const real tau_roll = 0.01 * (roll_ref - rpy(0)) - 0.0028 * rpy_rate(0);

			const real bound_d = 0.5 * mass * g;
			const real d_phi = Clamp(tau_roll / 2.0 / std::sqrt(2.0) / arm_length, -bound_d, bound_d);
			const real d_theta = Clamp(tau_pitch / 2.0 / std::sqrt(2.0) / arm_length, -bound_d, bound_d);
			const real d_psi = Clamp(tau_yaw / 4.0 / lambda, -bound_d, bound_d);
			thrusts[0] = 0.25 * total_thrust + d_phi + d_theta - d_psi;
			thrusts[1] = 0.25 * total_thrust - d_phi + d_theta + d_psi;
			thrusts[2] = 0.25 * total_thrust - d_phi - d_theta - d_psi;
			thrusts[3] = 0.25 * total_thrust + d_phi - d_theta + d_psi;
			// Clamping.
			for (int i = 0; i < 4; ++i)
			{
				thrusts[i] = Clamp(thrusts[i], 0.0, mass * g);
			}
		}

		for (int i = 0; i < 4; ++i) {
			thrust_vec[i] = thrusts[i] * -VectorD::UnitZ();
		}

		Advance_Rigid_Body(dt);

		// Post processing.
		rigid_body.R = rigid_body.orientation.toRotationMatrix();
		rigid_body.Rt = rigid_body.R.transpose();
		t += dt;
	}

	//////////////////////////////////////////////////////////////////////////
	////LV1: Rigid body simulation
	void Advance_Rigid_Body(const real dt)
	{
		const VectorD old_p = rigid_body.position;
		const VectorD old_v = rigid_body.velocity;
		const Eigen::AngleAxis<real> old_orientation = rigid_body.orientation;
		const VectorD old_omega = rigid_body.omega;
		const MatrixD old_R = rigid_body.R;

		////Linear motion
		// -- LV1 TASK: 3.1: update net force --
		VectorD net_force=VectorD::Zero();
		
		// -- Your implementation starts --
		// -- Your implementation ends --

		rigid_body.position = old_p + dt * old_v;
		rigid_body.velocity = old_v + dt * net_force/mass;

		////Angular motion
		// -- LV1 TASK 3.2: update net torque --
		VectorD net_torque = VectorD::Zero();
		
		// -- Your implementation starts --
		// -- Your implementation ends --
		VectorD n = old_omega * dt;
		real n_len = n.norm();
		if(n_len<1e-6)n=VectorD::UnitZ();
		else n/=n_len;
		rigid_body.orientation = Eigen::AngleAxis<real>(n_len, n) * old_orientation;

		VectorD old_body_omega = old_R.transpose() * old_omega;
		VectorD body_omega_rate = (net_torque - old_body_omega.cross(old_body_omega.cwiseProduct(inertia))).cwiseQuotient(inertia);
		rigid_body.omega = rigid_body.orientation.toRotationMatrix() * (old_body_omega + dt * body_omega_rate);
	}

	//////////////////////////////////////////////////////////////////////////
	////LV2: PD Controller
	
	Vector2 XyController(const real yaw, const Vector2& xy_ref, const Vector2& xy, const Vector2& v)
	{
		const Vector2 xy_error = xy_ref - xy;
		Matrix2 R = Matrix2::Identity();
		const real c = std::cos(yaw), s = std::sin(yaw);
		R(0, 0) = c; R(0, 1) = s; R(1, 0) = -s; R(1, 1) = c;
		Vector2 local_xy_error = R * xy_error;

		const Vector2 min_bound(-3.0, -3.0);
		const Vector2 max_bound(3.0, 3.0);
		local_xy_error = local_xy_error.cwiseMin(max_bound).cwiseMax(min_bound);
		const real x_err = local_xy_error.x();
		const real y_err = local_xy_error.y();
		const real P = 0.2;
		const real D = 0.25;
		const real dx_err = -v.x();
		const real dy_err = -v.y();
		real pitch = 0.0;
		real roll = 0.0;

		pitch = -P * x_err - D * dx_err;
		roll = P * y_err + D * dy_err;
		
		return Vector2(pitch, roll);
	}

	// -- TASK 3.3: Implement your altitude controller here --
	real AltitudeController(const real total_weight, const real z_ref, const real z, const real z_rate)
	{
		const real P_z = 0.2;
		const real D_z = 0.3;
		real total_control_thrust = 0.0;

		// -- Your implementation starts --
		// -- Your implementation ends --
		
		return total_control_thrust;
	}

	// -- TASK 3.4: Implement your yaw controller here --
	real YawController(const real yaw_ref, const real yaw, const real yaw_rate)
	{
		const real P_yaw = 0.004;
		const real D_yaw = 0.3 * 0.004;
		real total_control_torque = 0.0;
		
		// -- Your implementation starts --
		// -- Your implementation ends --

		return total_control_torque;
	}
};

#endif