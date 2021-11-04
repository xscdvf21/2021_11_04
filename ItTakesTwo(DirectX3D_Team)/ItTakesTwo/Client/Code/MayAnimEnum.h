#pragma once
#ifndef __MAYANIMENUM_H__

enum May_AnimID {
	May_Idle,                                     //0	frame:[61]
	May_Walk,                                     //1	frame:[21]
	May_Run,                                      //2	frame:[13]
	May_Jump,                                     //3	frame:[48]
	May_2ndJump,                                  //4	frame:[57]
	May_Jump_Fall,                                //5	frame:[19]
	May_Jump_Land,                                //6	frame:[14]
	May_Crouch_Start,                             //7	frame:[11]
	May_Crouch_Idle,                              //8	frame:[25]
	May_Crouch_End,                               //9	frame:[11]
	May_Crouch_Walk_Start,                        //10	frame:[15]
	May_Crouch_Walk,                              //11	frame:[29]
	May_Crouch_Walk_End,                          //12	frame:[9]
	May_Wall_Idle,                                //13	frame:[31]
	May_Wall_Jump,                                //14	frame:[19]
	May_Slide_Start,                              //15	frame:[11]
	May_Slide,                                    //16	frame:[21]
	May_Slide_End,                                //17	frame:[10]
	May_Slide_Left,                               //18	frame:[21]
	May_Slide_Right,                              //19	frame:[21]
	May_Fall,                                     //20	frame:[25]
	May_Fall_Oil,                                 //21	frame:[36]
	May_Push,                                     //22	frame:[31]
	May_Push_Idle,                                //23	frame:[61]
	May_Pull,                                     //24	frame:[31]
	May_Push_Button,                              //25	frame:[36]
	May_Jump_Dash,                                //26	frame:[31]
	May_Dash_Start,                               //27	frame:[14]
	May_Dash_End,                                 //28	frame:[19]
	May_Ledge_Start,                              //29	frame:[16]
	May_Ledge_Idle,                               //30	frame:[90]
	May_Ledge_ClimbUp,                            //31	frame:[25]
	May_Lever_RightToLeft,                        //32	frame:[36]
	May_Lever_LeftToRight,                        //33	frame:[31]
	May_GroundPound_Start,                        //34	frame:[16]
	May_GroundPound_Fall,                         //35	frame:[31]
	May_GroundPound_Land,                         //36	frame:[12]
	May_GroundPound_End,                          //37	frame:[19]
	May_Pickup,                                   //38	frame:[44]
	May_Pickup_Idle,                              //39	frame:[61]
	May_Pickup_Walk,                              //40	frame:[21]
	May_Pickup_Jump_Start,                        //41	frame:[48]
	May_Pickup_Jump_Fall,                         //42	frame:[19]
	May_Pickup_Jump_Land,                         //43	frame:[14]
	May_Pickup_PutDown,                           //44	frame:[21]
	May_Pickup_PutinSocket,                       //45	frame:[41]
	May_Hose_Start,                               //46	frame:[17]
	May_Hose_Idle,                                //47	frame:[29]
	May_Hose_Up,                                  //48	frame:[29]
	May_Hose_Left,                                //49	frame:[29]
	May_Hose_Right,                               //50	frame:[29]
	May_Hose_End,                                 //51	frame:[21]
	May_Hose_SuckedIn,                            //52	frame:[16]
	May_Hose_Inside,                              //53	frame:[21]
	May_Hose_BlowOut,                             //54	frame:[27]
	May_PowerSwitch_Start,                        //55	frame:[11]
	May_PowerSwitch_Idle,                         //56	frame:[41]
	May_PowerSwitch_Success,                      //57	frame:[26]
	May_VacuumLaunch_Start,                       //58	frame:[43]
	May_VacuumLaunch_IdleEnd,                     //59	frame:[19]
	May_VacuumLaunch_Idle,                        //60	frame:[19]
	May_Zerogravity_Idle,                         //61	frame:[31]
	May_LeverLeft,                                //62	frame:[23]
	May_LeverRight,                               //63	frame:[23]
	May_Wall_VerticalJump,                        //64	frame:[31]
	May_Valve_Idle,                               //65	frame:[66]
	May_Valve_RotateRight,                        //66	frame:[61]
	May_JumpDash_End,                             //67	frame:[26]
	May_Grind_Start,                              //68	frame:[20]
	May_Grind_Idle,                               //69	frame:[65]
	May_Grind_End,                                //70	frame:[17]
	May_CubeMove_Start,                           //71	frame:[11]
	May_CubeMove_End,                             //72	frame:[13]
	May_CubeMove_Idle,                            //73	frame:[84]
	May_CubeMove_Fwd,                             //74	frame:[16]
	May_CubeMove_Fwd_Right,                       //75	frame:[16]
	May_CubeMove_Right,                           //76	frame:[16]
	May_CubeMove_Bwd_Right,                       //77	frame:[16]
	May_CubeMove_Bwd,                             //78	frame:[16]
	May_CubeMove_Bwd_Left,                        //79	frame:[16]
	May_CubeMove_Left,                            //80	frame:[16]
	May_CubeMove_Fwd_Left,                        //81	frame:[16]
	May_Hit,                                      //82	frame:[20]
	May_End
};

#define __MAYANIMENUM_H__
#endif