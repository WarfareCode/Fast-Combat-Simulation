#pragma once
// --------------------------------------------------------------------------------------------------------------------------------
/**
*   @file           missile.h
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

#include "../Tools/coordinate.h"
#include "../FlyTac/aircraft.h"


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
	const Eigen::Vector3d angular_velocity);

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
	const Eigen::Vector3d angular_velocity);


// 
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
	const Eigen::Vector4d& in_handle);

// 
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
	Eigen::Vector4d* out_handle,
	double* errA_last,
	double* errP_last,
	double* errA_sum,
	double* errP_sum,
	const Eigen::Matrix4d& in_state,
	const double accelerator,
	const double in_timeslice,
	const Eigen::Vector3d target_point);