// --------------------------------------------------------------------------------------------------------------------------------
/**
*   @file           aircft.h
*   @brief          ����������ѧ
*   @details		��������ϵԭ��λ�ڲο��㣬X��ָ�򱱱ߣ�Y��ָ�򶫱ߣ�Z��ָ����¡�
					��������ϵԭ��λ�����ĵ㣬X��ָ���ͷǰ����Y��ָ���Ҳ࣬Z��ָ���·���
					��������ϵ��Ϊ����ϵ��
					��
					��
					��
					��
*   @author         LiDaiwei
*   @date           20191212
*   @version        1.0.0.1
*   @par Copyright
*                   LiDaiwei
*   @par History
*                   1.0.0.1: LiDaiwei, 20191212, �״δ���
*

*/

// --------------------------------------------------------------------------------------------------------------------------------
/**
*   @name           ͷ�ļ���
*   @{
*/

#include "../Tools/coordinate.h"



/** @}  */


/**
*   @name           ״̬�������塣
*   @{
*/
/*****
//�ɻ�״̬
Matrix4d state=[X,Y,Z,0;//λ��(��������ϵ)
			   Vx,Vy,Vz,0;//�ٶ�(��������ϵ)
			   q0,q1,q2,q3;//��̬(��Ԫ��)
			   0,0,0,0]//����

//���Ƹ���
Vector3d handle=[w_roll,//��ת���ٶ�(����/��)
				 w_pitch,//�������ٶ�(����/��)
				 w_yaw,//ƫ�����ٶ�(����/��)
				 a_body]//����

	  *****/
/** @}  */


// --------------------------------------------------------------------------------------------------------------------------------
/**
*   @brief          �ɽ��ٶ�����Ԫ����΢��
*   @details        �ɽ��ٶ�����Ԫ����΢��
*   @param[out]     dq              ��Ԫ����΢��:4ά����
*   @param[in]      q0              ��ʼ��Ԫ��:4ά����
*   @param[in]      wx              ������Ե���ϵ�Ľ��ٶ��ڻ���ϵX���ͶӰ
*   @param[in]      wy              ������Ե���ϵ�Ľ��ٶ��ڻ���ϵY���ͶӰ
*   @param[in]      wz              ������Ե���ϵ�Ľ��ٶ��ڻ���ϵZ���ͶӰ
*   @retval         0               ����
*   @retval         1               ����
*/
int angularvelocity_to_d_quaternion(
	Eigen::Vector4d* dq,
	const Eigen::Vector4d& q0,//����'&'�ᱨ������ __declspec(align('16')) ���βν���������
	const double wx,
	const double wy,
	const double wz);

// --------------------------------------------------------------------------------------------------------------------------------
/**
*   @name           ״̬�������塣
*   @{
*/
/*****
//�ɻ�״̬
Matrix4d state=[X,Y,Z,0;//λ��
			   Vx,Vy,Vz,0;//�ٶ�
			   q0,q1,q2,q3;//��̬(�ɻ���ϵ������ϵ����Ԫ��)
			   0,0,0,0]
//���Ƹ���
Vector3d handle=[w_roll,//��ת���ٶ�(����/��)
				 w_pitch,//�������ٶ�(����/��)
				 w_yaw,//ƫ�����ٶ�(����/��)
				 a_body]//����

	  *****/
	  /** @}  */


	  // --------------------------------------------------------------------------------------------------------------------------------
	  /**
	  *   @brief          ��״ֵ̬��״̬��΢��
	  *   @details        ��״ֵ̬��״̬��΢��
	  *   @param[out]     out_d_state           �ɻ�״̬
	  *   @param[in]      in_state              �ɻ�״̬
	  *   @param[in]      accelerator           ����
	  *   @param[in]      angular_velocity      ���ٶ�
	  *   @retval         0               ����
	  *   @retval         1               ����
	  */
int __f(
	Eigen::Matrix4d* out_d_state,
	const Eigen::Matrix4d& in_state,
	const double accelerator,
	const Eigen::Vector3d angular_velocity);

// --------------------------------------------------------------------------------------------------------------------------------
/**
*   @brief          �Ľ����������
*   @details        �Ľ����������
*   @param[out]     out_state             �ɻ�״̬
*   @param[in]      in_state              �ɻ�״̬
*   @param[in]      in_timeslice          ʱ����
*   @param[in]      accelerator           ����
*   @param[in]      angular_velocity      ���ٶ�
*   @retval         0               ����
*   @retval         1               ����
*/
int runge4(
	Eigen::Matrix4d* out_state,
	const Eigen::Matrix4d& in_state,
	const double in_timeslice,
	const double accelerator,
	const Eigen::Vector3d angular_velocity);


// 
// --------------------------------------------------------------------------------------------------------------------------------
/**
*   @brief          ����ɻ�״̬
*   @details        ����ɻ�״̬
*   @param[out]     out_state             �ɻ�״̬
*   @param[in]      in_state              �ɻ�״̬
*   @param[in]      in_timeslice          ʱ����
*   @param[in]      in_handle             ���������������ٶȣ���ת���ٶ�,���ţ�
*   @retval         0               ����
*   @retval         1               ����
*/
int Flight(
	Eigen::Matrix4d* out_state,
	const Eigen::Matrix4d& in_state,
	const double in_timeslice,
	const Eigen::Vector4d& in_handle);



// --------------------------------------------------------------------------------------------------------------------------------
/**
*   @brief          ��ת��PID
*   @details        ��ת��PID
*   @param[out]     dRoll_out             ���ֵ
*   @param[in]      error           ��ǰ���
*   @param[in]      inte_point      �ۻ����
*   @param[in]      dif_error_      ���΢��
*   @retval         0               ����
*   @retval         1               ����
*/
int PID_Roll(
	double* dRoll_out,
	const double error,
	const double inte_error,
	const double dif_error_);

// --------------------------------------------------------------------------------------------------------------------------------
/**
*   @brief          ������PID
*   @details        ������PID
*   @param[out]     dPitch_out             ���ֵ
*   @param[in]      error           ��ǰ���
*   @param[in]      inte_point      �ۻ����
*   @param[in]      dif_error_      ���΢��
*   @retval         0               ����
*   @retval         1               ����
*/
int PID_Pitch(
	double* dPitch_out,
	const double error,
	const double inte_error,
	const double dif_error_);

// --------------------------------------------------------------------------------------------------------------------------------
/**
*   @brief          ������PID
*   @details        ������PID
*   @param[out]     dYaw_out             ���ֵ
*   @param[in]      error           ��ǰ���
*   @param[in]      inte_point      �ۻ����
*   @param[in]      dif_error_      ���΢��
*   @retval         0               ����
*   @retval         1               ����
*/
int PID_Yaw(
	double* dYaw_out,
	const double error,
	const double inte_error,
	const double dif_error_);


// 
// --------------------------------------------------------------------------------------------------------------------------------
/**
*   @brief          �����
*   @details        PID���Ʒɻ������
*   @param[out]     out_handle            ���������������ٶȣ���ת���ٶ�,���ţ�
*   @param[out&in]  errA_last             ��һʱ�̷�λ�����
*   @param[out&in]  errP_last             ��һʱ�̸��������
*   @param[out&in]  errA_sum              �ۻ���λ�����
*   @param[out&in]  errP_sum              �ۻ����������
*   @param[in]      in_state              �ɻ�����״̬
*   @param[in]      accelerator           ����
*   @param[in]      in_timeslice          ʱ����
*   @param[in]      target_point          Ŀ���
*   @retval         0               ����
*   @retval         1               ����
*/
int Flight_find_point(
	Eigen::Vector4d* out_handle,
	double* errA_last,
	double* errP_last,
	double* errR_last,
	double* errA_sum,
	double* errP_sum,
	const Eigen::Matrix4d& in_state,
	const double accelerator,
	const double in_timeslice,
	const Eigen::Vector3d target_point);

