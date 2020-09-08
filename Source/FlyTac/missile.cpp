// --------------------------------------------------------------------------------------------------------------------------------
/**
*   @file           missile.cpp
*   @brief          �������м��Ƶ�
*   @details
					��
					��
					��
					��
*   @author         LiDaiwei
*   @date           20200201
*   @version        1.0.0.1
*   @par Copyright
*                   LiDaiwei
*   @par History
*                   1.0.0.1: LiDaiwei, 20200201, �״δ���
*

*/

// --------------------------------------------------------------------------------------------------------------------------------
/**
*   @name           ͷ�ļ���
*   @{
*/


#include "../FlyTac/missile.h"
#include <algorithm>

#include<iostream>
using namespace std;
using namespace Eigen;

/** @}  */


// --------------------------------------------------------------------------------------------------------------------------------
/**
*   @brief          ��״ֵ̬��״̬��΢��
*   @details        ��״ֵ̬��״̬��΢��
*   @param[out]     out_d_state           ����״̬
*   @param[in]      in_state              ����״̬
*   @param[in]      accelerator           ����
*   @param[in]      angular_velocity      ���ٶ�
*   @retval         0               ����
*   @retval         1               ����
*/
int __missile_f(
	Eigen::Matrix4d* out_d_state,
	const Eigen::Matrix4d& in_state,
	const double accelerator,
	const Eigen::Vector3d angular_velocity)
{
	Vector4d X0 = in_state.row(0),
		V0 = in_state.row(1),
		q0 = in_state.row(2);
	Vector4d dX, dV, dq;
	//��λ�õ�΢��
	dX = V0;

	//���ٶȵ�΢��
	Matrix3d Rnb, Rbn;

	quaternion_to_rotation(&Rbn, q0);
	Rnb = Rbn.transpose();
	double 	ax, ay, az;//��������

	Vector3d v_body,//����ϵ�ٶ�	
		v_n;//����ϵ�ٶ�
	v_n << V0(0), V0(1), V0(2);
	v_body = Rnb * v_n;

	ax = -(v_body(0) < 0 ? -1 : 1) * 8e-10 * fabs(pow(v_body(0), 4));//����
	ay = -(v_body(1) < 0 ? -1 : 1) * 1e-8 * fabs(pow(v_body(1), 4));//����
	az = -(v_body(2) < 0 ? -1 : 1) * 1e-6 * fabs(pow(v_body(2), 4));//����
	//����������ٶȷ����෴

	Vector3d Acceleration,//����ϵ���ٶ�
		acceleration;//����ϵ���ٶ�
	//cout << "vn" << v_n << endl << endl;
	//cout << "R" << Rnb << endl;
	//cout << "vb"<<v_body << endl << endl;
	if (X0(2) > 33000)
		acceleration << 0 + ax, ay,  az;
	else
		acceleration << accelerator + ax, ay,  az;

	//cout <<"a"<< acceleration << endl<<endl;
	Acceleration = Rbn * acceleration;
	Acceleration(2) = Acceleration(2) + simple_gravity;//������������
	dV << Acceleration(0), Acceleration(1), Acceleration(2), 0;
	//cout << dV << endl;
	//cin.get();
	for (int i = 0; i < 4; i++) {
		if (fabs(V0(i)) > 340 && fabs(dV(i)) > fabs(V0(i)) && dV(i) * V0(i) < 0)
			dV(i) = -V0(i);
	}
	//cout <<"dV" << Acceleration(0)<<"  "<< Acceleration(1) << "  " << Acceleration(2) << endl<<endl;


	//����Ԫ����΢��
	angularvelocity_to_d_quaternion(&dq, q0,
		angular_velocity(0), angular_velocity(1), angular_velocity(2));

	(*out_d_state).row(0) = dX;
	(*out_d_state).row(1) = dV;
	(*out_d_state).row(2) = dq;
	(*out_d_state).row(3) << 0, 0, 0, 0;

	return 0;
}

// --------------------------------------------------------------------------------------------------------------------------------
/**
*   @brief          �Ľ����������
*   @details        �Ľ����������
*   @param[out]     out_state             ����״̬
*   @param[in]      in_state              ����״̬
*   @param[in]      in_timeslice          ʱ����
*   @param[in]      accelerator           ����
*   @param[in]      angular_velocity      ���ٶ�
*   @retval         0               ����
*   @retval         1               ����
*/
int __missile_runge4(
	Eigen::Matrix4d* out_state,
	const Eigen::Matrix4d& in_state,
	const double in_timeslice,
	const double accelerator,
	const Eigen::Vector3d angular_velocity)
{
	double dt = in_timeslice;
	Matrix4d  k1, k2, k3, k4;						// for Runge-Kutta
	Matrix4d xn;

	// K1 = f(tn,xn)
	xn = in_state;
	__missile_f(&k1, xn, accelerator, angular_velocity);

	// K2 = f(t[n]+h/2,xn+h/2*K1)
	xn = in_state + 0.5 * dt * k1;
	__missile_f(&k2, xn, accelerator, angular_velocity);

	// K3 = f(t[n]+h/2,xn+h/2*K2)
	xn = in_state + 0.5 * dt * k2;
	__missile_f(&k3, xn, accelerator, angular_velocity);

	// K4 = f(t[n]+h/2,xn+h*K3)
	xn = in_state + dt * k3;
	__missile_f(&k4, xn, accelerator, angular_velocity);

	// x[n+1]=x[n]+h/6*(K1+2*K2+2*K3+K4)
	*out_state = in_state + dt * (k1 + 2 * k2 + 2 * k3 + k4) / 6.0;

	//��Ԫ����һ��
	Vector4d qa, qb;
	qa = (*out_state).row(2);
	quaterntion_normalized(&qb, qa);
	(*out_state).row(2) = qb;

	return 0;
}

// --------------------------------------------------------------------------------------------------------------------------------
/**
*   @brief          ���㵼��״̬
*   @details        ���㵼��״̬
*   @param[out]     out_state             ����״̬
*   @param[in]      in_state              ����״̬
*   @param[in]      in_timeslice          ʱ����
*   @param[in]      in_handle             ���������������ٶȣ���ת���ٶ�,���ţ�
*   @retval         0               ����
*   @retval         1               ����
*/
int missile_Flight(
	Eigen::Matrix4d* out_state,
	const Eigen::Matrix4d& in_state,
	const double in_timeslice,
	const Eigen::Vector4d& in_handle)
{
	Vector3d  angular_velocity;

	double wx, wy, wz, maxw, maxw2;
	maxw = M_PI_2;
	maxw2 = M_PI / 12;
	wx = (fabs(in_handle(0)) < maxw) ? in_handle(0) : (maxw * fabs(in_handle(0)) / in_handle(0));
	wy = (fabs(in_handle(1)) < maxw) ? in_handle(1) : (maxw * fabs(in_handle(1)) / in_handle(1));

	wz = (fabs(in_handle(2)) < maxw2) ? in_handle(2) : (maxw2 * fabs(in_handle(2)) / in_handle(2));


	angular_velocity << wx, wy, wz;//Wnbb:x-��תy-����z-ƫ��

	//angular_velocity << in_handle(1), in_handle(0), 0;//Wnbb:x-��תy-����z-ƫ��
	//cout << angular_velocity << endl << endl;

	__missile_runge4(out_state, in_state, in_timeslice, in_handle(3), angular_velocity);


	return 0;
}


// 
// --------------------------------------------------------------------------------------------------------------------------------
/**
*   @brief          ���������
*   @details        PID���Ƶ��������
*   @param[out]     out_handle            ���������������ٶȣ���ת���ٶ�,���ţ�
*   @param[out&in]  errA_last             ��һʱ�̷�λ�����
*   @param[out&in]  errP_last             ��һʱ�̸��������
*   @param[out&in]  errA_sum              �ۻ���λ�����
*   @param[out&in]  errP_sum              �ۻ����������
*   @param[in]      in_state              ��������״̬
*   @param[in]      accelerator           ����
*   @param[in]      in_timeslice          ʱ����
*   @param[in]      target_point          Ŀ���
*   @retval         0               ����
*   @retval         1               ����
*/
int missile_Flight_find_point(
	Vector4d* out_handle,
	double* errA_last,
	double* errP_last,
	double* errA_sum,
	double* errP_sum,
	const Matrix4d& in_state,
	const double accelerator,
	const double in_timeslice,
	const Vector3d target_point)
{
	Vector3d flight_point;
	flight_point << in_state(0, 0), in_state(0, 1), in_state(0, 2);
	Matrix3d Rnb, Rbn;
	quaternion_to_rotation(&Rbn, in_state.row(2));
	Rnb = Rbn.transpose();

	//����Ŀ�����Էɻ��ľ���
	double distance = (target_point - flight_point).norm();
	//cout << distance << endl;
	//cout << flight_point << endl << endl;
	//cout << target_point << endl << endl;

	//if (distance > 9900 || _isnan(distance)) {

	//	cin.get();
	//	cin.get();
	//}

	double roll = 0, pitch = 0, yaw = 90;
	quaternion_bn_to_euler(&roll, &pitch, &yaw, in_state.row(2));


	if (distance > 50) {
		//�ֱ����Ŀ�����Էɻ��ķ�λ�Ǻ͸����ǣ�
		Vector3d vector_flight_to_target;
		vector_flight_to_target = Rnb * (target_point - flight_point);
		//Ŀ�귽λ��
		double Azimuth = atan2(vector_flight_to_target(1), vector_flight_to_target(0));
		//Ŀ�긩����
		double Pitch = atan2(
			-vector_flight_to_target(2), sqrt(
				pow(vector_flight_to_target(0), 2) + pow(vector_flight_to_target(1), 2)));



		//�����ٶ�ʸ����Էɻ��ĵķ�λ�Ǻ͸����ǣ�
		Vector3d vector_velocity;
		vector_velocity << in_state(1, 0), in_state(1, 1), in_state(1, 2);
		vector_velocity = Rnb * vector_velocity;
		//�ٶȷ�λ��
		double Azimuth_V = atan2(vector_velocity(1), vector_velocity(0));
		//�ٶȸ�����
		double Pitch_V = atan2(
			-vector_velocity(2), sqrt(
				pow(vector_velocity(0), 2) + pow(vector_velocity(1), 2)));


		//cout << Rnb << endl << endl;

		//cout << flight_point << endl << endl;
		//cout << vector_flight_to_target << endl << endl;
		//cout << Azimuth << "  " << Pitch << " |||  " << Azimuth_V << "  " << Pitch_V << endl;

		//cin.get();

		//���������Ƕ����
		double errA = Azimuth - Azimuth_V;
		double errP = Pitch - Pitch_V;




		double d_roll = 0, d_pitch = 0, d_yaw = 0;

		*errA_sum += errA;
		*errP_sum += errP;

		PID_Roll(&d_roll, errA , *errA_sum, errA - (*errA_last));

		PID_Pitch(&d_pitch, errP, *errP_sum, errP - (*errP_last));
		PID_Yaw(&d_yaw, errA, *errA_sum, errA - (*errA_last));
		

		if (roll >= 170) {
			d_roll = -2;
		}
		if (roll <= -170) {
			d_roll = 2;
		}
		


		*errA_last = errA;
		*errP_last = errP;
		(*out_handle)(0) = d_roll;
		(*out_handle)(1) = d_pitch;
		(*out_handle)(2) = d_yaw;
		(*out_handle)(3) = accelerator;

	}
	else {
		(*out_handle)(0) = 0;
		(*out_handle)(1) = 0;
		(*out_handle)(2) = 0;
		(*out_handle)(3) = accelerator;


		*errA_last = 0;
		*errP_last = 0;

	}



	return 0;
}