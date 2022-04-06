


							   AREA MY_CODE,CODE,READONLY
							   ENTRY 
							   IMPORT switch_tasks
							   IMPORT save_stack_pointer
							   IMPORT get_stack_pointer_PSP
							   EXPORT initial_stack_handler_mode
		                       EXPORT thread_mode_using_psp
							   EXPORT PendSV_Handler

initial_stack_handler_mode  
                               MSR MSP,R0
                               BX LR
thread_mode_using_psp          
                               PRESERVE8 {TRUE}
							   PUSH{LR}
							   BL get_stack_pointer_PSP
							   MSR PSP,R0
							   POP{LR}
							   
							   MOV R0,#0x02
							   MSR CONTROL,R0
							   
                               BX LR

PendSV_Handler                
                              CPSID    I
	                          PRESERVE8 {TRUE}
							  MRS R0,PSP
						      STMDB R0!,{R4-R11}
							  PUSH {LR}
							  BL save_stack_pointer
							  BL switch_tasks
							  BL get_stack_pointer_PSP
							  LDMIA R0!,{R4-R11}
							  MSR PSP,R0
							  POP {LR}
								
							  CPSIE    I
								
							  BX LR
				   
				   
				   
				   
                               END