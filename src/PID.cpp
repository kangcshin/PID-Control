#include "PID.h"
#include <iostream>

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd) {
	PID::Kp = Kp;
	PID::Ki = Ki;
	PID::Kd = Kd;
	p_error = i_error = d_error = 0.0;
	dp = {0.1 * Kp, 0.1 * Ki, 0.1 * Kd};
	twiddle = false;
	t_step = 1;
	t_error = 0;
	b_error = std::numeric_limits<double>::max();
	n_steps_set = 50;
	n_steps_eval = 500;
	p_index = 2;
	q_add = false;
	q_subtract = false;
}

void PID::UpdateError(double cte) {
	if (t_step == 1) {
        p_error = cte;
    }
    d_error = cte - p_error;
    p_error = cte;
    i_error += cte;

    if (t_step % (n_steps_set + n_steps_eval) > n_steps_set){
        t_error += cte * cte;
    }

	if(twiddle && t_step % (n_steps_set + n_steps_eval) == 0){
		t_error += cte * cte;
	}

	if(twiddle && t_step % (n_steps_set + n_steps_eval) == 0){
		cout << "step: " << t_step << endl;
        cout << "total error: " << t_error << endl;
        cout << "best error: " << b_error << endl;
		if(t_error < b_error){
			b_error = t_error;
			if(t_step != n_steps_set + n_steps_eval){
				dp[p_index] *= 1.1;
			}
			p_index = 3 % (p_index + 1);
			q_add = q_subtract = false;
		}
		if(!q_add && !q_subtract){
			if (int(p_index) == 0) {
		        Kp += double(dp[p_index]);
		    }
		    else if (int(p_index) == 1) {
		        Kd += double(dp[p_index]);
		    }
		    else if (int(p_index) == 2) {
		        Ki += double(dp[p_index]);
		    }
		    else {
		    }

			q_add = true;
		}
		else if(q_add && !q_subtract){
			if (int(p_index) == 0) {
		        Kp += double(dp[p_index]);
		    }
		    else if (int(p_index) == 1) {
		        Kd += double(dp[p_index]);
		    }
		    else if (int(p_index) == 2) {
		        Ki += double(dp[p_index]);
		    }
		    else {
		    }
			q_subtract = true;
		}
		else{
			if (int(p_index) == 0) {
		        Kp += double(dp[p_index]);
		    }
		    else if (int(p_index) == 1) {
		        Kd += double(dp[p_index]);
		    }
		    else if (int(p_index) == 2) {
		        Ki += double(dp[p_index]);
		    }
		    else {
		    }
			dp[p_index] *= 0.9;
			p_index = 3 % (p_index + 1);
			q_add = q_subtract = false;
		}
		t_error = 0;
		cout << "new parameters" << endl;
        cout << "P: " << Kp << ", I: " << Ki << ", D: " << Kd << endl;   
	}
	t_step++;
}

double PID::TotalError() {
	return 0.0;
}

