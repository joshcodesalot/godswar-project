#pragma once

#define  VALIDATECODE_LENTH        25                    
#define  GAMESERVERNAME_LENGTH     32               
#define  ACCOUNTS_LENGTH           32               
#define  PASSWORD_LENGTH           32
#define  ROLENAME_LENGTH           32
#define	 MAX_HARDWAREINFO		   48
#define  MAC_LENGTH				   64	


//#define GW2
// #define GW2_USA

enum
{
	_OBJECT_TYPE_PLAYER				 = 0x10,
	_OBJECT_TYPE_NPC				 ,
	_OBJECT_TYPE_MONSTER			 ,
	_OBJECT_TYPE_BOSS				 ,
};

enum
{
	_MSG_INVALID		        	 =  0,

	_MSG_LOGIN                       =  1,
#if GW2_USA
	_MSG_FACEBOOK_LOGIN				 ,	
#endif
	_MSG_LOGIN_RETURN_INFO           ,                   
	_MSG_REQUEST_GAMESERVER          ,                   

	_MSG_VALIDATE_GAMESERVER         = 300,              

	_MSG_LOGIN_GAMESERVER            =  10000,           
	_MSG_RESPONSE_GAMESERVER         ,                   
	_MSG_ROLE_INFO                   ,                   
	_MSG_CREATE_ROLE                 ,                   
	_MSG_DELETE_ROLE                 ,                   
	_MSG_GAMESERVER_READY            ,                   
	_MSG_ENTER_GAME                  ,					 
	_MSG_CLIENT_READY                ,                   
	_MSG_GAMESERVER_INFO			 ,
	#if GW2
	_MSG_CHECK_INVITER_ID,
	_MSG_INVITER_RETURN				,
	_MSG_INVITER_RETURN2			,
	_MSG_INVITER_TIP,
#endif
	_MSG_SELFINFO                    ,				     
	_MSG_OBJECTINFO                  ,
	_MSG_LEAVE						 ,
	_MSG_COLONY_LEAVE                ,                   

	_MSG_WALK_BEGIN                  , 
	_MSG_WALK_END                    ,
	_MSG_SCENE_CHANGE                ,

	_MSG_FIGHT						 ,
	_MSG_ATTACK						 ,
	_MSG_DEAD						 ,
	_MSG_BACKHOME					 ,
	_MSG_DROPS						 ,
	_MSG_UPGRADE					 ,

	_MSG_KITBAG                      ,
	_MSG_STORAGE                     ,

	_MSG_TALK                        ,
	_MSG_TALKCHANNEL				 ,
	_MSG_PARAMATER                   ,
	_MSG_SKILL                       ,
	_MSG_ACTIVESKILL_INFO            ,
	_MSG_PASSIVESKILL_INFO           ,
	_MSG_SELFPROPERTY                ,
	_MSG_EFFECT                      ,
	_MSG_MAGIC_DAMAGE                ,
	_MSG_MAGIC_PERFORM				 ,
	_MSG_MAGIC_CLUSTER_DAMAGE		 ,


	_MSG_LEARN						 ,                   
	_MSG_SKILL_UPGRADE               ,                   

	_MSG_PICKUPDROPS				 ,					 
	_MSG_USEOREQUIP					 ,					 
	_MSG_MOVEITEM					 ,					 
	_MSG_BREAK_ITEM					 ,					 
	_MSG_STORAGEITEM				 ,					 
	_MSG_SELL						 ,					 

	_MSG_STALL						 ,					 
	_MSG_STALLADDITEM				 ,					 
	_MSG_STALLDELITEM				 ,					 
	_MSG_STALLITEM					 ,					 
	_MSG_STALLBUYITEM				 ,					 

	_MSG_TALKNPC					 ,					 
	_MSG_NPCDATA					 ,					 
	_MSG_SYS_NPC_DATA				 ,	    		     
	_MSG_SYS_FUN_USE                 ,                   
	_MSG_NPCITEMDATA				 ,					 
	_MSG_NPCSTORAGEDATA				 ,					 
	_MSG_NPCSELL					 ,					 

	_MSG_NPCQUEST					 ,					 
	_MSG_NPCNEXTQUEST				 ,					 
	_MSG_NPCQUESTS					 ,					 
	_MSG_NPCQUESTSAVAILABLE			 ,					 
	_MSG_NPCQUESTSUNAVAILABLE		 ,					 
	_MSG_NPCQUESTREWARD				 ,					 
	_MSG_NPCQUESTVIEW				 ,					 
	_MSG_NPCACCEPTQUEST				 ,					 
	_MSG_NPCCANCELQUEST				 ,					 
	_MSG_NPCCOMPLETEQUEST			 ,					 
	_MSG_NPCQUESTFAILD				 ,					 
	_MSG_NPCREWARDQUEST				 ,					 
	_MSG_NPCQUESTKILLORCAST			 ,					 
	_MSG_PLAYER_ACCEPTQUESTS		 ,					 
	_MSG_FINDQUEST					 ,					 
	_MSG_FINDQUESTRESULT			 ,					 


	_MSG_RESUNE                      ,

	_MSG_RELATIONALL                 ,
	_MSG_RELATION_REQUEST            ,
	_MSG_RELATION_RESPONSE           ,
	_MSG_RELATION_DELETE             ,
	_MSG_RELATION                    , 

	_MSG_TRADE						 ,
	_MSG_TRADE_MONEY				 ,
	_MSG_TRADE_ITEM					 ,
	_MSG_TRADE_ADDITEM				 ,
	_MSG_TRADE_REMOVEITEM			 ,

	_MSG_EQUIPFORGE					,
	_MSG_EQUIPFORGE_EQUIP			,
	_MSG_EQUIPFORGE_MATERIAL		,
	_MSG_EQUIPFORGE_EQUIPCANCEL		,
	_MSG_EQUIPFORGE_MATERIALCANCEL	,
	_MSG_EQUIPFORGE_CANCEL			,

	_MSG_GOLD_BOX					,

	_MSG_EXPLORER_QUEST				,					

	_MSG_GOLD_BOX_RETURN				,

	_MSG_TEAM_INVITE				,		       
	_MSG_TEAM_REQUEST				,
	_MSG_TEAM_INFO					,
	_MSG_TEAM_ADD					,		  
	_MSG_TEAM_DELETE				,		  		
	_MSG_TEAM_REPLACELEADER			,		  
	_MSG_TEAM_DISSOLVE				,		  
	_MSG_TEAM_LEAVE					,		  
	_MSG_TEAM_TIP					,		  
	_MSG_TEAM_REJECT				,		  
	_MSG_TEAM_REFLASH				,		  
	_MSG_TEAM_DESTROY				,		  

	_MSG_UPDATE_MP					,

	_MSG_CONSORTIA_CREATE           ,        
	_MSG_CONSORTIA_CREATE_RESPONSE  ,         
	_MSG_CONSORTIA_BASE_INFO        ,        
	_MSG_CONSORTIA_MEMBER_LIST      ,        
	_MSG_CONSORTIA_INVITE           ,        
	_MSG_CONSORTIA_DISMISS          ,        
	_MSG_CONSORTIA_RESPONSE         ,        
	_MSG_CONSORTIA_EXIT             ,        
	_MSG_CONSORTIA_TEXT             ,        
	_MSG_CONSORTIA_DUTY             ,        
	_MSG_CONSORTIA_MEMBER_DEL       ,        

	_MSG_ALTAR_INFO					,		 

	_MSG_ERROR						,
	_MSG_MANAGE_RETURN              ,

	_MSG_SKILLPOINT_UPGRADE         ,

	_MSG_SYN_GAMEDATA               ,

	_MSG_STATUS           ,

	_MSG_LOGIN_QUEUE,

	_MSG_SERVER_NOTE,
	_MSG_SKILLBACKUP,
	_MSG_SKILL_INTERRUPT             ,                   

	_MSG_KEY_RETURN,                                    
	_MSG_BAN_PLAYER,
	_MSG_CONSORTIA_LVUP,	                           
	_MSG_ALTAR_CREATE,	                               
	_MSG_ALTAR_LVUP,		                          
	_MSG_ALTAR_OBLATION,	                          

	_MSG_MALLITEMDATA,				    
	_MSG_ASSOCIATIONITEMDATA,           
	_MSG_MALLSELL,					    
	_MSG_ASSOCIATIONSELL,			    
	_MSG_ASSOCIATIONDISCOUNT,		    
	_MSG_CRETIT_EXCHANGE             ,                   
	_MSG_QUESTEXPLORERRESULT		,					 
	_MSG_SYS_ADD_ITEM		,					 
	_MSG_SYS_DEL_ITEM		,					
	_MSG_COUNT,
	_MSG_TARGETINFO,
	_MSG_DELAY_EXIT,								

	_MSG_WALK,


};    


enum {
	TARGET_SELF				= 1,		 
	TARGET_SAME_FACTION		= 2,		 
	TARGET_DIFF_FACTION		= 4,		 
	TARGET_MONSTER			= 8,		 
	TARGET_POS				= 16,		 
	TARGET_PK_OBJ			= 32,		 
	TARGET_ASHES            = 64         
};

#define MAX_CONSORTIA_MEMBER_COUNT 200        

#define					defWHISPER				1
#define					defTEAM					2
#define					defGUILD				4
#define					defCAMP					8
#define					defLOCAL				16	
#define					defMESSAGE				32

#define					defSYSTEM				129
#define					defWORLD				130
#define					defWHISPER_SELF			131
#define					defGM					132
#define					DIALITEMCOUNT			16				
