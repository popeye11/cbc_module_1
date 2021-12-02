/*******************************************************************************
 *
 * @copyright   TOPTICA Photonics AG
 * @date        2020
 *
 * @file        tsp_pid.cpp
 * @brief       MATLAB S-function for the PID.
 *              See matlabroot/simulink/src/sfuntmpl_doc.c for a more detailed
 *              template.
 *
 * @author      Fuchs, Daniel <Daniel.Fuchs@toptica.com>
 * @author      Hager, Manfred <Manfred.Hager@toptica.com>
 * @author      Hempel, Felix <Felix.Hempel@toptica.com>
 * @author      Lopes, Emilio <Emilio.Lopes@toptica.com>
 * @author      Rehme, Paul <Paul.Rehme@toptica.com>
 * @author      Roggenbuck, Axel <Axel.Roggenbuck@toptica.com>
 * @author      Zhang, Xiaodong <Xiaodong.Zhang@toptica.com>
 *
 ******************************************************************************/
#include <tsp/pid.hpp>

#include <iostream>

#define S_FUNCTION_NAME  tsp_pid
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"


/*******************************************************************************
 * @brief                   The sizes information is used by Simulink to
 *                          determine the S-function block's characteristics
 *                          (number of inputs, outputs, states, etc.).
 * 
 * @param S                 The simulation structure.
 ******************************************************************************/
static void mdlInitializeSizes(SimStruct *S) {
    ssSetNumSFcnParams(S, 0);
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) return;

    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);

    if (!ssSetNumInputPorts(S, 5)) return;
    ssSetInputPortWidth(S, 0, 1);
    ssSetInputPortWidth(S, 1, 1);
    ssSetInputPortWidth(S, 2, 1);
    ssSetInputPortWidth(S, 3, 1);
    ssSetInputPortWidth(S, 4, 1);

    ssSetInputPortDirectFeedThrough(S, 0, 1);
    ssSetInputPortDirectFeedThrough(S, 1, 1);
    ssSetInputPortDirectFeedThrough(S, 2, 1);
    ssSetInputPortDirectFeedThrough(S, 3, 1);
    ssSetInputPortDirectFeedThrough(S, 4, 1);

    if (!ssSetNumOutputPorts(S, 1)) return;
    ssSetOutputPortWidth(S, 0, 1);
    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumPWork(S, 1);
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);

    /* Specify the sim state compliance to be same as a built-in block */
    ssSetSimStateCompliance(S, USE_DEFAULT_SIM_STATE);

    ssSetOptions(S, 0);
}

/*******************************************************************************
 * @brief                   This function is used to specify the sample time(s)
 *                          for your S-function. You must register the same
 *                          number of sample times as specified in
 *                          ssSetNumSampleTimes.
 * 
 * @param S                 The simulation structure.
 ******************************************************************************/
static void mdlInitializeSampleTimes(SimStruct *S) {   
    ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, 0.0);
    ssSetModelReferenceSampleTimeDefaultInheritance(S);
}

#define MDL_INITIALIZE_CONDITIONS
#if defined(MDL_INITIALIZE_CONDITIONS)
/*******************************************************************************
 * @brief                   In this function, you should initialize the
 *                          continuous and discrete states for your S-function
 *                          block.  The initial states are placed in the state
 *                          vector, ssGetContStates(S) or
 *                          ssGetRealDiscStates(S). You can also perform any
 *                          other initialization activities that your
 *                          S-function may require. Note, this routine will be
 *                          called at the start of simulation and if it is
 *                          present in an enabled subsystem configured to reset
 *                          states, it will be call when the enabled subsystem
 *                          restarts execution to reset the states.
 * 
 * @param S                 The simulation structure.
 ******************************************************************************/
static void mdlInitializeConditions(SimStruct *S) {}
#endif

#define MDL_START
#if defined(MDL_START)
/*******************************************************************************
 * @brief                   This function is called once at start of model
 *                          execution. If you have states that should be
 *                          initialized once, this is the place to do it.
 * 
 * @param S                 The simulation structure.
 ******************************************************************************/
static void mdlStart(SimStruct *S) {
    auto pid{new toptica::tsp::pid::pid<real_T>{ssGetSampleTime(S,0)}};

    ssGetPWork(S)[0] = (void *)pid;

    pid->enable();
}
#endif

/*******************************************************************************
 * @brief                   In this function, you compute the outputs of your
 *                          S-function block.
 * 
 * @param S                 The simulation structure.
 * @param tid               ?
 ******************************************************************************/
static void mdlOutputs(SimStruct *S, int_T tid) {
    UNUSED_ARG(tid);

    auto pid{(toptica::tsp::pid::pid<real_T> *)ssGetPWork(S)[0]};

    pid->set_p(*ssGetInputPortRealSignalPtrs(S,0)[0]);
    pid->set_i(*ssGetInputPortRealSignalPtrs(S,1)[0]);
    pid->set_d(*ssGetInputPortRealSignalPtrs(S,2)[0]);

    *ssGetOutputPortRealSignal(S,0) =
        pid->run(
            *ssGetInputPortRealSignalPtrs(S,3)[0] -
                *ssGetInputPortRealSignalPtrs(S,4)[0]);
}

#define MDL_UPDATE
#if defined(MDL_UPDATE)
/*******************************************************************************
 * @brief                   This function is called once for every major
 *                          integration time step. Discrete states are typically
 *                          updated here, but this function is useful for
 *                          performing any tasks that should only take place
 *                          once per integration step.
 * 
 * @param S                 The simulation structure.
 * @param tid               ?
 ******************************************************************************/
static void mdlUpdate(SimStruct *S, int_T tid) {}
#endif

#define MDL_DERIVATIVES
#if defined(MDL_DERIVATIVES)
/*******************************************************************************
 * @brief                   In this function, you compute the S-function block's
 *                          derivatives. The derivatives are placed in the
 *                          derivative vector, ssGetdX(S).
 * 
 * @param S                 The simulation structure.
  ******************************************************************************/
static void mdlDerivatives(SimStruct *S) {}
#endif

/*******************************************************************************
 * @brief                   In this function, you should perform any actions
 *                          that are necessary at the termination of a
 *                          simulation. For example, if memory was allocated in
 *                          mdlStart, this is the place to free it.
 * 
 * @param S                 The simulation structure.
 ******************************************************************************/
static void mdlTerminate(SimStruct *S) {
    delete (toptica::tsp::pid::pid<real_T> *)ssGetPWork(S)[0];
}

#ifdef MATLAB_MEX_FILE     /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
