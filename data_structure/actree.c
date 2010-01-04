/************************************************************************/
/*	
	Copyright to AI INFINITY, Inc.
	Solution designed by Ben Shaw.
	Programmed by Ben Shaw.
	$Reversion 1.0 $ $Data: 2006/12/19 $ Created by Ben Shaw
*/
/************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "tool.h"
#include "actree.h"



typedef struct _FcnRegNode
{
    int				Tag;
    LOGICAL			Enable;
    MG_REGION		GeometricRegion;
    int				ActiveMapAmount;
    ACTION_MAP_PTR	ActionFcnMap;

    struct _FcnRegNode* NextFcnRegion;
    struct _FcnRegNode* PreviousFcnRegion;
}FUNCTION_REGION_NODE,*FUNCTION_REGION_NODE_PTR;

typedef FUNCTION_REGION_NODE_PTR FUNCTION_REGION_LIST;

typedef struct  
{
    int                     FunRegAmount;
    FUNCTION_REGION_LIST    FunRegList;
}FUNCTION_REGIONS_HEAD,*FUNCTION_REGIONS_HEAD_PTR;


typedef struct  
{
    /* Register component amount */
    int				ComRegAmount;
    MG_REGION		GridRegion;
    int				normalstepx;
    int				normalstepy;
    /* The row amount of the region list array */
    int				row;				
    /* The column amount of the region list array */
    int				column;
    /* The pointer to Abstract Component List Array */			
    void**			ACGListArray;		

}MG_GRID,*MG_GRID_PTR;

typedef struct _AbstractComponentNode
{
	MG_HANDLE                       MGHandle;
    MG_REGION                       GeometricRegion;
    FUNCTION_REGIONS_HEAD_PTR       FunctionRegion;
	int								ChildrenAmount;
    MG_GRID_PTR						ChildrenGrid;
    struct _AbstractComponentNode*	father;
    struct _AbstractComponentNode*	NextBrother;
	struct _AbstractComponentNode*	PreviousBrother;
    struct _AbstractComponentNode*	children;
}ABSTRACT_COMPONENT_NODE,*ABSTRACT_COMPONENT_NODE_PTR;

typedef ABSTRACT_COMPONENT_NODE_PTR ABSTRACT_COMPONENT_TREE;
typedef ABSTRACT_COMPONENT_NODE_PTR AC_BROTHER_LIST;
typedef ABSTRACT_COMPONENT_NODE_PTR AC_CHILDREN_LIST;

typedef struct _AbstractComponentGeometricNode 
{
	MG_REGION								GeometricRegion;
	ABSTRACT_COMPONENT_NODE_PTR				ACNPtr;
	
	struct _AbstractComponentGeometricNode* NextNode;
	struct _AbstractComponentGeometricNode* PreviousNode;
}ABSTRACT_COMPONENT_GEOMETRIC_NODE,*ABSTRACT_COMPONENT_GEOMETRIC_NODE_PTR;

typedef ABSTRACT_COMPONENT_GEOMETRIC_NODE_PTR ABSTRACT_COMPONENT_GEOMETRIC_LIST;

typedef struct  
{
    int                                     ACGNAmount;
    MG_REGION								GridGeometricRegion;
    ABSTRACT_COMPONENT_GEOMETRIC_LIST       ACGList;
}ABSTRACT_COMPONENT_GEOMETRIC_HEAD,*ABSTRACT_COMPONENT_GEOMETRIC_HEAD_PTR;

typedef ABSTRACT_COMPONENT_GEOMETRIC_HEAD_PTR* ACG_LIST_ARRAY;

typedef struct  
{
    int     ACTHandleType;
    void*   ACTEntryPointer;
}ACT_HANDLE_ENTRY,*ACT_HANDLE_ENTRY_PTR;


/* Insert a child node to a abstract component children list */
static int insertChild2List(
							ABSTRACT_COMPONENT_TREE actree
							,ABSTRACT_COMPONENT_NODE_PTR acnodeptr
				);

ACT_HANDLE CreateACTHandle(int HandleType)
{
    ACT_HANDLE_ENTRY_PTR ACTEntryPtr;
    ACT_HANDLE ACTHandle;

    if ((ACTEntryPtr=malloc(sizeof(ACT_HANDLE_ENTRY)))==ACT_NULL)
    {
        printf("Memory limited!\n");
        return ACT_NULL;
    }

    memset(ACTEntryPtr,ACT_NULL,sizeof(ACT_HANDLE_ENTRY));

    ACTEntryPtr->ACTHandleType=HandleType;
    ACTEntryPtr->ACTEntryPointer=ACT_NULL;
    ACTHandle=(ACT_HANDLE)ACTEntryPtr;

    return ACTHandle;
}

LOGICAL IsACTHandleEmpty(ACT_HANDLE handle)
{
    ACT_HANDLE_ENTRY_PTR ACTHandleEntryPtr;

    if (handle==ACT_NULL)
    {
        return True;
    }
    
    ACTHandleEntryPtr=(ACT_HANDLE_ENTRY_PTR)handle;

    return ACTHandleEntryPtr->ACTEntryPointer==ACT_NULL?True:False;
}

/************************************************************************/
/* Routines for initializing every element of Abstract Component Tree   */
/************************************************************************/

/* Initial Abstract Component Tree Node */
int initActree(
                ACT_HANDLE ACTHandle,
                MG_HANDLE ACTMGHandle,
                MG_REGION_PTR MGRegionPtr
               )
{
	ABSTRACT_COMPONENT_TREE actree;
    ACT_HANDLE_ENTRY_PTR ACTHandleEntryPtr;

    if (ACTHandle==ACT_NULL)
    {
        return ACT_ERROR;
    }

    ACTHandleEntryPtr=(ACT_HANDLE_ENTRY_PTR)ACTHandle;
    if (ACTHandleEntryPtr->ACTHandleType!=ACT_ACN_TYPE)
    {
        printf("Handle type mismatch!\n");
        return ACT_ERROR;
    }

	if ((actree=(ABSTRACT_COMPONENT_TREE)
		malloc(sizeof(ABSTRACT_COMPONENT_NODE)))==NULL)
	{
		printf("Memory limited!\n");
		return ACT_ERROR;
	}
	
    memset(actree,ACT_NULL,sizeof(ABSTRACT_COMPONENT_NODE));
    actree->father=ACT_NULL;
    actree->NextBrother=ACT_NULL;
	actree->PreviousBrother=ACT_NULL;
    actree->children=ACT_NULL;
    actree->ChildrenGrid=ACT_NULL;
    actree->FunctionRegion=ACT_NULL;
	actree->ChildrenAmount=0;
    actree->MGHandle=ACTMGHandle;
    actree->GeometricRegion.bottom=MGRegionPtr->bottom;
    actree->GeometricRegion.left=MGRegionPtr->left;
    actree->GeometricRegion.right=MGRegionPtr->right;
    actree->GeometricRegion.top=MGRegionPtr->top;
    
    ACTHandleEntryPtr->ACTEntryPointer=(void*)actree;
    
	return ACT_OK;
}

/* Initial function region list */
int initialFcnRegList(ACT_HANDLE ACTHandle)
{
    FUNCTION_REGIONS_HEAD_PTR FcnRegHead;
    ACT_HANDLE_ENTRY_PTR ACTEntryPtr;
    ABSTRACT_COMPONENT_NODE_PTR ACNPtr;
 
    if (IsACTHandleEmpty(ACTHandle)==True)
    {
        printf("Empty ACT handle!\n");
        return ACT_ERROR;
    }
    
    ACTEntryPtr=(ACT_HANDLE_ENTRY_PTR)ACTHandle;

    if (ACTEntryPtr->ACTHandleType!=ACT_ACN_TYPE)
    {
        printf("ACT Handle type mismatch!\n");
        return ACT_ERROR;
    }

    ACNPtr=(ABSTRACT_COMPONENT_NODE_PTR) ACTEntryPtr->ACTEntryPointer;

    if ((FcnRegHead=(FUNCTION_REGIONS_HEAD_PTR)
        malloc(sizeof(FUNCTION_REGIONS_HEAD)))==ACT_NULL)
    {
        printf("Memory limited!\n");
        return ACT_ERROR;
    }
    
    FcnRegHead->FunRegAmount=0;
    FcnRegHead->FunRegList=ACT_NULL;

    ACNPtr->FunctionRegion=FcnRegHead;

    return ACT_OK;
}

/* Initial children grid */
int initializeGrid(
                ACT_HANDLE ACTHandle, 
                MG_REGION_PTR MGRegionPtr,
                int RowAmount,
                int ColAmount,
                )
{
    MG_GRID_PTR GridPtr;
	ACT_HANDLE_ENTRY_PTR ACTHandleEntryPtr;
	ABSTRACT_COMPONENT_TREE actree;
    ACG_LIST_ARRAY ACGListArray;
    ABSTRACT_COMPONENT_GEOMETRIC_HEAD_PTR ACGHeadPtr;
    int i,j,gleft,gright,gbottom,gtop,cleft,cright,cbottom,ctop;

	if (IsACTHandleEmpty(ACTHandle)==True)
	{
        return ACT_ERROR;
	}
    ACTHandleEntryPtr=(ACT_HANDLE_ENTRY_PTR)ACTHandle;
	
	if (ACTHandleEntryPtr->ACTHandleType!=ACT_ACN_TYPE)
	{
		return ACT_ERROR;
	}

	actree=(ABSTRACT_COMPONENT_TREE)ACTHandleEntryPtr->ACTEntryPointer;
	
    if ((GridPtr=(MG_GRID_PTR)malloc(sizeof(MG_GRID)))==ACT_NULL)
    {
        printf("Memory limited!\n");
        return ACT_ERROR;
    }
    
    GridPtr->column=ColAmount;
    GridPtr->row=RowAmount;
    GridPtr->GridRegion.bottom=MGRegionPtr->bottom;
    GridPtr->GridRegion.left=MGRegionPtr->left;
    GridPtr->GridRegion.right=MGRegionPtr->right;
    GridPtr->GridRegion.top=MGRegionPtr->top;
   
    gbottom=GridPtr->GridRegion.bottom;
    gtop=GridPtr->GridRegion.top;
    gleft=GridPtr->GridRegion.left;
    gright=GridPtr->GridRegion.right;

    GridPtr->normalstepx=(MGRegionPtr->right-MGRegionPtr->left+1)/ColAmount;
    GridPtr->normalstepy=(MGRegionPtr->top-MGRegionPtr->bottom+1)/RowAmount;
	
	if ((GridPtr->ACGListArray=malloc(RowAmount*ColAmount*sizeof(void *)))==ACT_NULL)
	{
		printf("Memory limited!\n");
		return ACT_ERROR;
	}
	
	memset(GridPtr->ACGListArray, ACT_NULL, RowAmount*ColAmount*sizeof(void *));
    
    ACGListArray=(ACG_LIST_ARRAY)GridPtr->ACGListArray;
    /*
     *	Initialize ACG List Array
     */
    for (i=0;i<RowAmount;i++)
    {
        for (j=0;j<ColAmount;j++)
        {
            if ((ACGHeadPtr=(ABSTRACT_COMPONENT_GEOMETRIC_HEAD_PTR)
                malloc(sizeof(ABSTRACT_COMPONENT_GEOMETRIC_HEAD)))==ACT_NULL)
            {
                printf("Memory limited!\n");
                return ACT_ERROR;
            }
            memset(ACGHeadPtr,0,sizeof(ABSTRACT_COMPONENT_GEOMETRIC_HEAD));
            ACGHeadPtr->ACGList=ACT_NULL;
            ACGHeadPtr->ACGNAmount=0;
            cleft=gleft+j*GridPtr->normalstepx;
            cright=cleft+GridPtr->normalstepx;
            cright=cright>gright?gright:cright;
            cbottom=gbottom+(RowAmount-i)*GridPtr->normalstepy;
            ctop=cbottom+GridPtr->normalstepy;
            ctop=ctop>gtop?gtop:ctop;

            ACGHeadPtr->GridGeometricRegion.left=cleft;
            ACGHeadPtr->GridGeometricRegion.right=cright;
            ACGHeadPtr->GridGeometricRegion.bottom=cbottom;
            ACGHeadPtr->GridGeometricRegion.top=ctop;

            *(ACGListArray+i*ColAmount+j)=ACGHeadPtr;

        }
    }
    
	GridPtr->ComRegAmount=0;
	
	actree->ChildrenGrid=GridPtr;

	return ACT_OK;
}

/************************************************************************/
/* Routines for creating Abstract Component Tree                        */
/************************************************************************/

/* Insert a child node to a abstract component children list */
static int insertChild2List(
				ABSTRACT_COMPONENT_TREE FatherACT
				,ABSTRACT_COMPONENT_NODE_PTR ChildACT
				)
{
	ABSTRACT_COMPONENT_NODE_PTR CurrentNodePtr,PreviousNodePtr;
	
	if (FatherACT==ACT_NULL || ChildACT==ACT_NULL)
	{
		return ACT_ERROR;
	}

    ChildACT->father=FatherACT;

	/* link */
	if (FatherACT->children==ACT_NULL)	/* First Child */
	{
		FatherACT->children=ChildACT;
		ChildACT->NextBrother=ACT_NULL;
		ChildACT->PreviousBrother=ACT_NULL;
		FatherACT->ChildrenAmount++;
	}
	else
	{
		CurrentNodePtr=FatherACT->children;
		FatherACT->children=ChildACT;
		ChildACT->NextBrother=CurrentNodePtr;
		ChildACT->PreviousBrother=ACT_NULL;
		CurrentNodePtr->PreviousBrother=ChildACT;
		FatherACT->ChildrenAmount++;
	}

	return ACT_OK;
}

static int  insertChild2ACGList(
                                ABSTRACT_COMPONENT_GEOMETRIC_HEAD_PTR ACGHeadPtr,
                                ABSTRACT_COMPONENT_GEOMETRIC_NODE_PTR ACGNPtr
                                )
{
    ABSTRACT_COMPONENT_GEOMETRIC_NODE_PTR InsertNodePtr, CurrentNodePtr;

    if ((InsertNodePtr=(ABSTRACT_COMPONENT_GEOMETRIC_NODE_PTR)
        malloc(sizeof(ABSTRACT_COMPONENT_GEOMETRIC_NODE)))==ACT_NULL)
    {
        printf("Memory limited!\n");
        return ACT_ERROR;
    }

    InsertNodePtr->ACNPtr=ACGNPtr->ACNPtr;
    InsertNodePtr->GeometricRegion=ACGNPtr->GeometricRegion;

    CurrentNodePtr=ACGHeadPtr->ACGList;
    ACGHeadPtr->ACGList=InsertNodePtr;
    InsertNodePtr->NextNode=CurrentNodePtr;
    InsertNodePtr->PreviousNode=ACT_NULL;
    CurrentNodePtr->PreviousNode=InsertNodePtr;

    ACGHeadPtr->ACGNAmount++;
    return ACT_OK;
}

/* Insert a Abstract Component Node to the grid */
/*
*	The coordinate of the insertion AC node must be the relativity 
*	coordinate to his father
*/
static int insertGridACNode(
							ABSTRACT_COMPONENT_TREE FatherACT
							,ABSTRACT_COMPONENT_NODE_PTR ChildACT
							)
{
	ABSTRACT_COMPONENT_GEOMETRIC_LIST* ACGListArray;
	ABSTRACT_COMPONENT_GEOMETRIC_LIST ACGList;
    ABSTRACT_COMPONENT_GEOMETRIC_NODE_PTR ChildACGNPtr;
	MG_GRID_PTR ChildrenGrid;
	int left,right,bottom,top,i,j,row1,row2,col1,col2;
	int gleft,gright,gbottom,gtop,normalstepx,normalstepy,row,column;

	if (FatherACT==ACT_NULL || ChildACT==ACT_NULL)
	{
		return ACT_ERROR;
	}
	
	ChildrenGrid=FatherACT->ChildrenGrid;
    
    ChildACGNPtr->ACNPtr=ChildACT;
    ChildACGNPtr->GeometricRegion=ChildACT->GeometricRegion;

	left=ChildACGNPtr->GeometricRegion.left;
	right=ChildACGNPtr->GeometricRegion.right;
	bottom=ChildACGNPtr->GeometricRegion.bottom;
	top=ChildACGNPtr->GeometricRegion.top;

	gleft=ChildrenGrid->GridRegion.left;
	gright=ChildrenGrid->GridRegion.right;
	gbottom=ChildrenGrid->GridRegion.bottom;
	gtop=ChildrenGrid->GridRegion.top;

	normalstepx=ChildrenGrid->normalstepx;
	normalstepy=ChildrenGrid->normalstepy;

	row=ChildrenGrid->row;
	column=ChildrenGrid->column;

	ACGListArray=(ABSTRACT_COMPONENT_GEOMETRIC_LIST*)ChildrenGrid->ACGListArray;

	/* Parameter check */
	if (left>right || top < bottom)
	{
		printf("Insertion coordinate error!\n");
		return ACT_ERROR;
	}
	if (gleft > gright || gtop < gbottom)
	{
		printf("Global coordinate error!\n");
		return ACT_ERROR;
	}
	if (left>gright || right<gleft || bottom>gtop || top<gbottom)
	{
		printf("Component fully over border!\n");
		return ACT_ERROR;
	}

	/* Regular */
	left=left < 0 ? 0 : left;
	bottom=bottom <0 ? 0 : bottom;
	right=right > gright-gleft ? gright-gleft : right;
	top=top > gtop-gbottom ? gtop-gbottom : top;
	row1=(gtop-gbottom-top)/normalstepy;
	row2=(gtop-gbottom-bottom)/normalstepy-((gtop-gbottom-bottom)%normalstepy==0);
	col1=left/normalstepx;
	col2=right/normalstepx-(right%normalstepx==0);

	for (i=row1;i<= row2;i++)
	{
		for (j=col1;j<=col2;j++)
		{
			if (insertChild2ACGList(*(ACGListArray+i*column+j),ChildACGNPtr)==ACT_ERROR)
			{
                return ACT_ERROR;
			}
			
		}
	}

	return ACT_OK;
}

int insertChild(
                ACT_HANDLE FatherHandle,
                ACT_HANDLE ChildHandle
                )
{
    ABSTRACT_COMPONENT_NODE_PTR ChildACT, FatherACT;
    ACT_HANDLE_ENTRY_PTR ACTFatherHandleEntryPtr, ACTChildHandleEntryPtr;
    
    if (IsACTHandleEmpty(FatherHandle)==True || 
        IsACTHandleEmpty(ChildHandle)==ACT_NULL)
    {
        return ACT_ERROR;
    }
    
    ACTFatherHandleEntryPtr=(ACT_HANDLE_ENTRY_PTR)FatherHandle;
    ACTChildHandleEntryPtr=(ACT_HANDLE_ENTRY_PTR)ChildHandle;

    if (
        ACTFatherHandleEntryPtr->ACTHandleType!=ACT_ACN_TYPE
        || ACTChildHandleEntryPtr->ACTHandleType!=ACT_ACN_TYPE
        )
    {
        printf("Type mismatch!\n");
        return ACT_ERROR;
    }

    
    FatherACT=(ABSTRACT_COMPONENT_NODE_PTR)ACTFatherHandleEntryPtr->ACTEntryPointer;
    ChildACT=(ABSTRACT_COMPONENT_NODE_PTR)ACTChildHandleEntryPtr->ACTEntryPointer;


    if (insertChild2List(FatherACT,ChildACT)==ACT_ERROR)
    {
        printf("Insert Child ACT to children list error!\n");
        return ACT_ERROR;
    }
    
    if (insertGridACNode(FatherACT,ChildACT)==ACT_ERROR)
    {
        printf("Insert child ACGN to children grid error!\n");
        return ACT_ERROR;
    }

    return ACT_OK;
}

int insertFcnRegNode(
					 ACT_HANDLE	ACTHandle,
					 FCN_REG_INFO_PTR FcnRegInfoPtr
					 )
{
	FUNCTION_REGION_LIST FcnRegList;
	FUNCTION_REGION_NODE_PTR FcnRegNodeNew, CurrentNodePtr;
	ACT_HANDLE_ENTRY_PTR ACTHandleEntryPtr;
	ABSTRACT_COMPONENT_NODE_PTR ACNPtr;
	FUNCTION_REGIONS_HEAD_PTR FcnHeadPtr;

	if (IsACTHandleEmpty(ACTHandle)==True)
	{
		printf("Empty ACT Handle!\n");
		return ACT_ERROR;
	}
	
	ACTHandleEntryPtr=(ACT_HANDLE_ENTRY_PTR)ACTHandle;
	if (ACTHandleEntryPtr->ACTHandleType!=ACT_ACN_TYPE)
	{
		printf("ACT type mismatch!\n");
		return ACT_ERROR;
	}

	ACNPtr=ACTHandleEntryPtr->ACTEntryPointer;

	
	if ((FcnRegNodeNew=(FUNCTION_REGION_NODE_PTR)
		malloc(sizeof(FUNCTION_REGION_NODE)))==NULL)
	{
		printf("Memory limited!\n");
		return ACT_ERROR;
	}

	/* Evaluation */
	FcnRegNodeNew->Tag=FcnRegInfoPtr->Tag;
	FcnRegNodeNew->ActionFcnMap=FcnRegInfoPtr->ActionFcnMap;
	FcnRegNodeNew->ActiveMapAmount=FcnRegInfoPtr->ActiveMapAmount;
	FcnRegNodeNew->Enable=FcnRegInfoPtr->Enable;
	FcnRegNodeNew->GeometricRegion=FcnRegInfoPtr->GeometricRegion;
	
	FcnHeadPtr=ACNPtr->FunctionRegion;
	/* Relink */
	if (FcnHeadPtr->FunRegList==ACT_NULL)	/* First node */
	{
		FcnRegNodeNew->PreviousFcnRegion=ACT_NULL;
		FcnRegNodeNew->NextFcnRegion=ACT_NULL;
		FcnHeadPtr->FunRegList=FcnRegNodeNew;
		FcnHeadPtr->FunRegAmount++;
	}
	else
	{
		CurrentNodePtr=FcnHeadPtr->FunRegList;
		FcnRegNodeNew->PreviousFcnRegion=ACT_NULL;
		FcnRegNodeNew->NextFcnRegion=CurrentNodePtr;
		CurrentNodePtr->PreviousFcnRegion=FcnRegNodeNew;
		FcnHeadPtr->FunRegList=FcnRegNodeNew;
		FcnHeadPtr->FunRegAmount++;
	}

	return ACT_OK;
}



/* Find a Abstract Component Node in brother list */
static ABSTRACT_COMPONENT_NODE_PTR findBrotherPre(
	ABSTRACT_COMPONENT_TREE ACTree
	,MG_HANDLE handle
	)
{
	ABSTRACT_COMPONENT_NODE_PTR CurrentNodePtr,PreviousNodePtr;

	if (ACTree==NULL)
	{
		return NULL;
	}
	PreviousNodePtr=ACTree->children;
	CurrentNodePtr=PreviousNodePtr;
	while (CurrentNodePtr)
	{
		if (CurrentNodePtr->MGHandle==handle)
		{
			if (CurrentNodePtr==PreviousNodePtr)	/* First Match */
			{
				return ACTree;
			}
			else	/* Normal match */
			{
				return PreviousNodePtr;
			}	
		}
		PreviousNodePtr=CurrentNodePtr;
		CurrentNodePtr=CurrentNodePtr->NextBrother;
	}
	return NULL;
}					




/*
 *	Delete a Abstract Tree and all its children
 *	If current Abstract Component Node is a shadow object, just delete its
 *	brother, children and itself. Otherwise, if it has grid, delete the 
 *	grid and all its members; If it has brithers delete, all of them; If it 
 *	has children, delete all of them.
 */
void deleteTree(ABSTRACT_COMPONENT_TREE actree)
{
	ABSTRACT_COMPONENT_TREE* actreearray;
	int row,column,i;

	if (actree==NULL)
	{
		return;
	}
	if (actree->ObjectType==OBJECT_TYPE_INITIAL_VALUE) 
	{
		deleteTree(actree->children);
		actree->children=NULL;
		free(actree);
		return;
	}

	if (actree->ObjectType==REAL_OBJECT_ID)	/* Real Object */
	{
		/* If abstract component tree have children grid */
		if (actree->ChildrenGrid!=NULL)
		{
			row=actree->ChildrenGrid->row;
			column=actree->ChildrenGrid->column;
			actreearray=(ABSTRACT_COMPONENT_TREE*)
				actree->ChildrenGrid->ACGListArray;
			/* Delete all the children which is registered to the grid */
			if (actreearray!=NULL)
			{
				for (i=0;i<row*column;i++)
				{
					deleteTree(actreearray[i]);
				}
				/* Free the array */
				free(actreearray);
			}
			
			/* Free the grid structure */
			free(actree->ChildrenGrid);
			actree->ChildrenGrid=NULL;
		}
		/* If own brother, delete all its brothers */
		if (actree->NextBrother)
		{
			deleteTree(actree->NextBrother);
			actree->NextBrother=NULL;
		}
		/* If own children list delete all the children */
		if (actree->children)	/* own children */
		{
			deleteTree(actree->children);
			actree->children=NULL;
		}
		/* End node */
		if (actree->ChildrenGrid==NULL 
			&& actree->NextBrother==NULL
			&& actree->children==NULL)
		{
			/* Free function region */
			if (actree->FunctionRegion)
			{
				deleteFcnRegList(actree->FunctionRegion);
			}
			/* Free the tree node */
			free(actree);
			return;
			
		}
	}
	else if (actree->ObjectType==SHADOW_OBJECT_ID)	/* Shadow object */
	{
		/* If own brother, delete brother */
		if (actree->NextBrother!=NULL)	/* own brother */
		{
			deleteTree(actree->NextBrother);
			actree->NextBrother=NULL;
		}
		/* If own children list delete all the children */
		if (actree->children!=NULL)	/* own children */
		{
			deleteTree(actree->children);
			actree->children=NULL;
		}
		/* End node */
		if (actree->ChildrenGrid==NULL 
			&& actree->NextBrother==NULL 
			&& actree->children==NULL)
		{
			/* Free the tree node */
			free(actree);
			return;
		}
	}
	return;
}

/*
 *	Delete all the children of a abstract component, include the Children
 *	List and Children Grid.
 */
void delChildren(ABSTRACT_COMPONENT_TREE actree)
{
	ABSTRACT_COMPONENT_TREE* actreearray;
	int row,column,i;

	if (actree==NULL)
	{
		return;
	}
	if (actree->ObjectType==REAL_OBJECT_ID)
	{
		if (actree->children)
		{
			/* delete Children list */
			deleteTree(actree->children);
			actree->children=NULL;
		}
		if (actree->ChildrenGrid)
		{
			/* Delete Children Grid */
			row=actree->ChildrenGrid->row;
			column=actree->ChildrenGrid->column;
			actreearray=(ABSTRACT_COMPONENT_TREE*)
				actree->ChildrenGrid->ACGListArray;
			if (actreearray!=NULL)
			{
				for (i=0;i<row*column;i++)
				{
					deleteTree(actreearray[i]);
				}
				/* Free AC tree array */
				free(actree->ChildrenGrid->ACGListArray);
			}
			
			/* Free Grid data structure */
			free(actree->ChildrenGrid);
			actree->ChildrenGrid=NULL;
		}

	}
	else if (actree->ObjectType==SHADOW_OBJECT_ID)
	{
		if (actree->children)
		{
			/* delete Children list */
			deleteTree(actree->children);
			actree->children=NULL;
		}
	}
	
	return;
}

/*
 *	Delete a child of the father node, use handle as the match tag.
 *	Delete all of the real and Shadow object.
 *  The input abstract component must be a real object.
 */
int delChild_handle(ABSTRACT_COMPONENT_TREE actree,MG_HANDLE handle)
{
	ABSTRACT_COMPONENT_TREE* actreearray;
	ABSTRACT_COMPONENT_NODE_PTR ChildACNodePtr;
	int left,right,bottom,top,i,j,row1,row2,col1,col2;
	int gleft,gright,gbottom,gtop,normalstepx,normalstepy,row,column;
	
	if (actree==NULL || handle==NULL)
	{
		return MG_ERROR;
	}
	if (actree->children==NULL)	/* No children */
	{
		return MG_OK;
	}
	ChildACNodePtr=findachcl(actree,handle);
	if (actree->children)
	{
		if (delChildinList(actree,handle)==MG_ERROR)
		{
			printf("Delete child node from grid fail!\n");
		}
	}
		
	/* Delete all the shadow object if existence */
	if (actree->ChildrenGrid)	/* Exist Grid */
	{
		left=ChildACNodePtr->GeometricRegion.left;
		right=ChildACNodePtr->GeometricRegion.right;
		bottom=ChildACNodePtr->GeometricRegion.bottom;
		top=ChildACNodePtr->GeometricRegion.top;
		gleft=actree->GeometricRegion.left;
		gright=actree->GeometricRegion.right;
		gbottom=actree->GeometricRegion.bottom;
		gtop=actree->GeometricRegion.top;
		normalstepx=actree->ChildrenGrid->normalstepx;
		normalstepy=actree->ChildrenGrid->normalstepy;
		row=actree->ChildrenGrid->row;
		column=actree->ChildrenGrid->column;
		actreearray=(ABSTRACT_COMPONENT_TREE*)actree->ChildrenGrid->ACGListArray;
		/* Parameter check */
		if (left>right || top > bottom)
		{
			printf("Insertion coordinate error!\n");
			return MG_ERROR;
		}
		if (gleft > gright || gtop > gbottom)
		{
			printf("Global coordinate error!\n");
			return MG_ERROR;
		}
		if (left>gright-gleft || right<0 || bottom>gbottom-gtop || top<0)
		{
			printf("Component fully over border!\n");
			return MG_ERROR;
		}
		left=left < 0 ? 0 : left;
		bottom=bottom <0 ? 0 : bottom;
		right=right > gright-gleft ? gright-gleft : right;
		top=top > gtop-gbottom ? gtop-gbottom : top;
		row1=(gtop-gbottom-top)/normalstepy;
		row2=(gtop-gbottom-bottom)/normalstepy-((gtop-gbottom-bottom)%normalstepy==0);
		col1=left/normalstepx;
		col2=right/normalstepx-(right%normalstepx==0);
		for (i=row1;i<= row2;i++)
		{
			for (j=col1;j<=col2;j++)
			{
				if (delChildinList(actreearray[i*column+j],handle)==MG_ERROR)
				{
					printf("Delete child node from grid fail!\n");
				}
			}
		}
	}
	
	return MG_OK;
	
}


/*
 *	Delete a child from the children list.
 */
int delChildinList(
				 ABSTRACT_COMPONENT_TREE actree
				,MG_HANDLE handle
				)
{
	ABSTRACT_COMPONENT_NODE_PTR CurrentNodePtr,PreviousNodePtr;
	
	/* Parameter check */
	if (actree==NULL)
	{
		return MG_ERROR;
	}
	if (actree->children==NULL)
	{
		return MG_ERROR;
	}

	PreviousNodePtr=actree->children;
	CurrentNodePtr=actree->children;
	while (CurrentNodePtr)
	{
		if (CurrentNodePtr->MGHandle==handle)
		{
			if (PreviousNodePtr==CurrentNodePtr)	/* First child match */
			{
				/* Relink */
				actree->children=CurrentNodePtr->NextBrother;
				CurrentNodePtr->NextBrother=NULL;
				deleteTree(CurrentNodePtr);
			}
			else	/* Normal match */
			{
				/* relink */
				PreviousNodePtr->NextBrother=CurrentNodePtr->NextBrother;
				CurrentNodePtr->NextBrother=NULL;
				deleteTree(CurrentNodePtr);
			}
			return MG_OK;
		}
		PreviousNodePtr=CurrentNodePtr;
		CurrentNodePtr=CurrentNodePtr->NextBrother;
	}
	printf("No match error!\n");
	return MG_ERROR;
	
}

/*
 *	Find a child in children list, first match.
 */
/*
 *	Coordinate has been converted by caller function
 */
static ABSTRACT_COMPONENT_NODE_PTR findChildinList(
									  ABSTRACT_COMPONENT_TREE actree
									  ,COORDINATE wp
									  )
{
	ABSTRACT_COMPONENT_NODE_PTR CurrentNodePtr;
	
	if (actree==NULL)
	{
		return NULL;
	}
	CurrentNodePtr=actree->children;
	while (CurrentNodePtr)
	{
		if (isInRegion(&(CurrentNodePtr->GeometricRegion),&wp))
		{
			return CurrentNodePtr;
		}
		CurrentNodePtr=CurrentNodePtr->NextBrother;
	}
	return NULL;
}

/*
 *	Find a Child with Geometric Coordinate
 *	This function will return a child AC object and converts
 *	the world coordinate to local relative coordinate.	
 */
ABSTRACT_COMPONENT_NODE_PTR findChild_gc(
									  ABSTRACT_COMPONENT_TREE actree
									  , COORDINATE_PTR wp
									  )
{
	COORDINATE lp,ep;
	ABSTRACT_COMPONENT_TREE * actreearray;
	ABSTRACT_COMPONENT_NODE_PTR MatchEntry=NULL;
	int normalstepx,normalstepy,i,j,row,column,left,right,bottom,top;
	MG_REGION region;

	
	if (actree==NULL || isInRegion(&(actree->GeometricRegion),wp)==False)
	{
		return NULL;
	}
	ep.x=actree->GeometricRegion.left;
	ep.y=actree->GeometricRegion.bottom;
	world2local(&ep,wp);
	lp.x=wp->x;
	lp.y=wp->y;
	/* Search Grid first */
	if (actree->ChildrenGrid)	/* Exist grid */
	{                                                                                                                                                                       
		normalstepx=actree->ChildrenGrid->normalstepx;
		normalstepy=actree->ChildrenGrid->normalstepy;
		row=actree->ChildrenGrid->row;
		column=actree->ChildrenGrid->column;
		region=actree->ChildrenGrid->GridRegion;
		left=region.left;
		right=region.right;
		bottom=region.bottom;
		top=region.top;
		i=(top-bottom-lp.y)/normalstepy;
		j=lp.x/normalstepx;
		actreearray=actree->ChildrenGrid->ACGListArray;
		if (i<row && j<column)
		{
			MatchEntry=findChildinList(actreearray[i*column+j],lp);
		}
	}
	/* Search in children list */
	if (actree->children)	/* Exist children list */
	{
		MatchEntry=findChildinList(actree,lp);
	}
	return MatchEntry;
}
/*
 *	Find End Object with World Geometric Coordinate
 *	The function will return a End AC Object and convert
 *	the world coordinate to local relative coordinate
 */
ABSTRACT_COMPONENT_NODE_PTR feogc(
								  ABSTRACT_COMPONENT_TREE actree
								  ,COORDINATE_PTR wp
								 )
{
	ABSTRACT_COMPONENT_NODE_PTR CurrentNodePtr,MatchNodePtr;

	CurrentNodePtr=actree;
	while ((MatchNodePtr=findChild_gc(CurrentNodePtr,wp))!=NULL)
	{
		CurrentNodePtr=MatchNodePtr;
	}
	return CurrentNodePtr;
}



/*
 *	Find a abstract component with handle from the Children List (The brothers)
 */
ABSTRACT_COMPONENT_NODE_PTR findachcl(
										 ABSTRACT_COMPONENT_TREE actree,
										 MG_HANDLE handle
										 )
{
	ABSTRACT_COMPONENT_NODE_PTR CurrentACNodePtr;

	if (actree==NULL)
	{
		return NULL;
	}
	CurrentACNodePtr=actree->children;
	while (CurrentACNodePtr)
	{
		if (CurrentACNodePtr->MGHandle==handle)
		{
			return CurrentACNodePtr;
		}
		CurrentACNodePtr=CurrentACNodePtr->NextBrother;
	}
	return NULL;
	
}

/*
 *	Move a Abstract Component to the top in the grid
 *	This routine will move the Abstract Component to the top of every 
 *	AC list which is registered to the grid. 
 */
int moveACtop_ACGrid(MG_GRID_PTR grid,ABSTRACT_COMPONENT_NODE_PTR ACNodePtr)
{
	void** ACtreeArray;
	int row,column,normalstepx,normalstepy,left,right,bottom,top,i,j;
	int gleft,gright,gbottom,gtop,row1,row2,col1,col2;
	ABSTRACT_COMPONENT_TREE actree;
	ABSTRACT_COMPONENT_NODE_PTR CurrentNodePtr,PreviousNodePtr;

	if (grid==NULL || ACNodePtr==NULL)
	{
		return MG_ERROR;
	}

	ACtreeArray=grid->ACGListArray;
	row=grid->row;
	column=grid->column;
	normalstepx=grid->normalstepx;
	normalstepy=grid->normalstepy;
	left=ACNodePtr->GeometricRegion.left;
	right=ACNodePtr->GeometricRegion.right;
	bottom=ACNodePtr->GeometricRegion.bottom;
	top=ACNodePtr->GeometricRegion.top;
	gleft=grid->GridRegion.left;
	gright=grid->GridRegion.right;
	gbottom=grid->GridRegion.bottom;
	gtop=grid->GridRegion.top;
	row1=(gtop-gbottom-top)/normalstepy;
	row2=(gtop-gbottom-bottom)/normalstepy-((gtop-gbottom-bottom)%normalstepy==0);
	col1=left/normalstepx;
	col2=right/normalstepx-(right%normalstepx==0);
	for (
		i=row1;
		i < row2;
		i++
		)
	{
		for (j=col1;j<col2;j++)
		{
			actree=ACtreeArray[i*column+j];
			if ((PreviousNodePtr=findBrotherPre(actree,ACNodePtr->MGHandle)))
			{
				if (PreviousNodePtr!=actree && PreviousNodePtr!=NULL)
				{
					CurrentNodePtr=PreviousNodePtr->NextBrother;
					PreviousNodePtr->NextBrother=CurrentNodePtr->NextBrother;
					CurrentNodePtr->NextBrother=actree->children;
					actree->children=CurrentNodePtr;
				}
			}
			
		}	
	}
	return MG_OK;

}
/*
 *	Facility resource
 */
/*
 *	Get world absolute region coordinate
 */
MG_REGION getwarc(ABSTRACT_COMPONENT_NODE_PTR ACNodePtr)
{
	MG_REGION region;
	ABSTRACT_COMPONENT_NODE_PTR FatherACNodePtr;
	int width,height,x,y;

	region.left=0;
	region.right=-1;
	if (ACNodePtr==NULL)
	{
		return region;
	}
	region=ACNodePtr->GeometricRegion;
	width=region.right-region.left+1;
	height=region.top-region.bottom+1;
	x=region.left;
	y=region.bottom;
	FatherACNodePtr=ACNodePtr->parent;
	while (FatherACNodePtr)
	{
		x=x+FatherACNodePtr->GeometricRegion.left;
		y=y+FatherACNodePtr->GeometricRegion.bottom;
		FatherACNodePtr=FatherACNodePtr->parent;
	}
	region.left=x;
	region.bottom=y;
	region.right=x+width-1;
	region.top=y+height-1;
	return region;
}




/* Insert a node to the head of the list */




/*
*	Find Previous Function Region
*	local invoke function
*	The input position must be local relative coordinate
*/
static FUNCTION_REGION_NODE_PTR findFcnRegPre(
    FUNCTION_REGION_LIST FcnRegList,
    COORDINATE position
    )
{
    FUNCTION_REGION_NODE_PTR CurrentNodePtr,PreviousNodePtr;

    if (FcnRegList==NULL)
    {
        return NULL;
    }
    CurrentNodePtr=FcnRegList->NextFcnRegion;
    PreviousNodePtr=FcnRegList;
    while (CurrentNodePtr!=NULL)
    {
#ifdef _DEBUG111
        printf(
            "(L:%d,R:%d,B:%d,T%d)\n"
            ,CurrentNodePtr->GeometricRegion.left
            ,CurrentNodePtr->GeometricRegion.right
            ,CurrentNodePtr->GeometricRegion.bottom
            ,CurrentNodePtr->GeometricRegion.top
            );
        printf("(%d %d)\n",position.x,position.y);
#endif // _DEBUG111
        if (isInRegion(&(CurrentNodePtr->GeometricRegion),&position))
        {
            return PreviousNodePtr;
        }
        PreviousNodePtr=CurrentNodePtr;
        CurrentNodePtr=CurrentNodePtr->NextFcnRegion;
    }
    return NULL;
}

static FUNCTION_REGION_NODE_PTR findFcnRegPre_Tag(
    FUNCTION_REGION_LIST FcnRegList,
    int Tag
    )
{
    FUNCTION_REGION_NODE_PTR CurrentNodePtr,PreviousNodePtr;

    if (FcnRegList==NULL)
    {
        return NULL;
    }
    CurrentNodePtr=FcnRegList->NextFcnRegion;
    PreviousNodePtr=FcnRegList;
    while (CurrentNodePtr!=NULL)
    {
        if (CurrentNodePtr->Tag==Tag)
        {
            return PreviousNodePtr;
        }
        PreviousNodePtr=CurrentNodePtr;
        CurrentNodePtr=CurrentNodePtr->NextFcnRegion;
    }
    return CurrentNodePtr;
}

/* Delete a node from the list */
/*
*	The input position must be relative coordinate
*/
void deleteFcnRegNode(
                      ABSTRACT_COMPONENT_NODE_PTR ACNodePtr,
                      COORDINATE position
                      )
{
    FUNCTION_REGION_NODE_PTR CurrentNodePtr,PreviousNodePtr;
    FUNCTION_REGION_LIST FcnRegList;

    if (ACNodePtr==NULL)
    {
        return;
    }
    FcnRegList=ACNodePtr->FunctionRegion;
    if (FcnRegList==NULL)
    {
        return;
    }
    if ((PreviousNodePtr=findFcnRegPre(FcnRegList,position))==NULL)
    {
        printf("Can not find the function region!\n");
        return;
    }
    CurrentNodePtr=PreviousNodePtr->NextFcnRegion;
    /* Relink */
    PreviousNodePtr->NextFcnRegion=CurrentNodePtr->NextFcnRegion;
    /* Free current node */
    if (CurrentNodePtr->ActionFcnMap!=NULL)
    {
        free(CurrentNodePtr->ActionFcnMap);
    }
    if (CurrentNodePtr!=NULL)
    {
        free(CurrentNodePtr);
    }
    return;

}

/* Delete a function region node according to a integer Tag */
void deleteFcnRegNode_Tag(
                          FUNCTION_REGION_LIST FcnRegList,
                          int Tag
                          )
{
    FUNCTION_REGION_NODE_PTR CurrentNodePtr,PreviousNodePtr;

    if ((PreviousNodePtr=findFcnRegPre_Tag(FcnRegList,Tag))==NULL)
    {
        printf("Can not find the function region!\n");
        return;
    }
    CurrentNodePtr=PreviousNodePtr->NextFcnRegion;
    /* Relink */
    PreviousNodePtr->NextFcnRegion=CurrentNodePtr->NextFcnRegion;
    /* Free current node */
    if (CurrentNodePtr->ActionFcnMap!=NULL)
    {
        free(CurrentNodePtr->ActionFcnMap);
    }
    if (CurrentNodePtr!=NULL)
    {
        free(CurrentNodePtr);
    }
    return;
}

void deleteFcnRegList(FUNCTION_REGION_LIST FcnRegList)
{
    FUNCTION_REGION_NODE_PTR CurrentNodePtr,Tmp;
    /* Protection */
    if (FcnRegList==NULL)
    {
        return;
    }

    CurrentNodePtr=FcnRegList->NextFcnRegion;
    while (CurrentNodePtr!=NULL)
    {
        Tmp=CurrentNodePtr->NextFcnRegion;
        /* Free Action map array */
        if (CurrentNodePtr->ActionFcnMap!=NULL)
        {
            free(CurrentNodePtr->ActionFcnMap);
        }
        CurrentNodePtr->ActionFcnMap=NULL;
        free(CurrentNodePtr);
        CurrentNodePtr=Tmp;
    }
    return;

}

/*
*	Find a function region according to a coordinate
*	The region must be the first match
*	The input coordinate must be local relative coordinate
*/
FUNCTION_REGION_NODE_PTR findFcnReg(ABSTRACT_COMPONENT_NODE_PTR ACNodePtr,
                               COORDINATE position)
{
    FUNCTION_REGION_NODE_PTR NodePtr;
    FUNCTION_REGION_LIST FcnRegList;

    if (ACNodePtr==NULL)
    {
        return NULL;
    }
    FcnRegList=ACNodePtr->FunctionRegion;
    if (FcnRegList==NULL)
    {
        return NULL;
    }
    if ((NodePtr=findFcnRegPre(FcnRegList,position))==NULL)
    {
        return NULL;
    }
    return NodePtr->NextFcnRegion;
}

/*
*	Find a function region according to a integer tag
*	The region must be the first match
*/
FUNCTION_REGION_NODE_PTR findFcnReg_Tag(FUNCTION_REGION_LIST FcnRegList,
                                   int Tag)
{
    FUNCTION_REGION_NODE_PTR NodePtr;

    if ((NodePtr=findFcnRegPre_Tag(FcnRegList,Tag))==NULL)
    {
        printf("Tag mismatch!\n");
        return NodePtr;
    }
    return NodePtr->NextFcnRegion;
}

/*
*	Active all Function Region
*	The input variable "uievent" (User interaction event) contain the
*	position information of component in local relative coordinate
*/
void activeAllFcnReg(
                     ABSTRACT_COMPONENT_NODE_PTR	CurrentACNodePtr
                     ,COORDINATE_PTR				localposition
                     ,UI_EVENT_PTR					uievent
                     )
{
    FUNCTION_REGION_LIST FcnRegList;
    FUNCTION_REGION_NODE_PTR CurrentFcnRegPtr;
    int i;
    void (*CallbackFcnPtr)(void*,void*,void*,void*);
    ACTION_MAP_PTR ActionMapPtr;
    MOUSE_EVENT_PTR MouseEvent;

    if (CurrentACNodePtr==NULL)
    {
        return;
    }
    FcnRegList=CurrentACNodePtr->FunctionRegion;
    if (FcnRegList==NULL)
    {
        return;
    }
    CurrentFcnRegPtr=FcnRegList->NextFcnRegion;	/* jump head node */
    MouseEvent=uievent->MouseEvent;
    while (CurrentFcnRegPtr)
    {
        if (CurrentFcnRegPtr->Enable)
        {
            if (isInRegion(&(CurrentFcnRegPtr->GeometricRegion),localposition))
            {
                ActionMapPtr=CurrentFcnRegPtr->ActionFcnMap;
                if (CurrentFcnRegPtr->ActiveMapAmount>0 && ActionMapPtr!=NULL)
                {

                    for (i=0;i<CurrentFcnRegPtr->ActiveMapAmount;i++)
                    {
                        switch(uievent->ActiveDevice)
                        {
                        case VIRTUAL_DEV_ID:
                            if (ActionMapPtr[i].ActiveDevice==VIRTUAL_DEV_ID&&
                                ActionMapPtr[i].ActiveType==uievent->VirtualEvent
                                )	/* compare Virtual Event */
                            {
                                CallbackFcnPtr=ActionMapPtr[i].ActiveFcn;
                                (*CallbackFcnPtr)(CurrentACNodePtr,CurrentFcnRegPtr,localposition,uievent);
                            }
                            break;
                        case DEV_MOUSE_ID:
                            if (ActionMapPtr[i].ActiveDevice==DEV_MOUSE_ID&&
                                ActionMapPtr[i].ActiveType==MouseEvent->mosevent)	/* compare mouse event */
                            {
                                CallbackFcnPtr=ActionMapPtr[i].ActiveFcn;
                                (*CallbackFcnPtr)(CurrentACNodePtr,CurrentFcnRegPtr,localposition,uievent);
                            }
                            break;
                        case DEV_KEYBOARD_ID:
                            if (ActionMapPtr[i].ActiveDevice==DEV_KEYBOARD_ID&&
                                ActionMapPtr[i].ActiveType==uievent->KBEvent)		/* Compare keyboard event */
                            {
                                CallbackFcnPtr=ActionMapPtr[i].ActiveFcn;
                                (*CallbackFcnPtr)(CurrentACNodePtr,CurrentFcnRegPtr,localposition,uievent);
                            }
                        default:
                            printf("Unsupported device!\n");
                            break;
                        }
                    }
                }
            }

        }
        CurrentFcnRegPtr=CurrentFcnRegPtr->NextFcnRegion;	/* Point to next Function Region */
    }

}

/*
*	Active First Match Function Region
*	The input coordinate must be in local relative position
*/
void activefmfr(
                ABSTRACT_COMPONENT_NODE_PTR ACNodePtr
                ,COORDINATE_PTR				localposition
                ,UI_EVENT_PTR				uievent
                )
{
    int							i;
    ACTION_MAP_PTR				ActionMap;
    MOUSE_EVENT_PTR				MouseEventPtr;
    FUNCTION_REGION_NODE_PTR			FcnRegPtr;
    void (*CallbackFcnPtr)(void*,void*,void*,void*);

    if (ACNodePtr==NULL)
    {
        return ;
    }
    MouseEventPtr=uievent->MouseEvent;
    FcnRegPtr=findFcnReg(ACNodePtr,*localposition);
    ActionMap=FcnRegPtr->ActionFcnMap;
    if (FcnRegPtr==NULL || FcnRegPtr->Enable==False)
    {
        return;
    }
    /* Active all of the matched action entries */
    for (i=0;i<FcnRegPtr->ActiveMapAmount;i++)
    {
        if (ActionMap[i].ActiveDevice==uievent->ActiveDevice)
        {
            switch(uievent->ActiveDevice)
            {
            case VIRTUAL_DEV_ID:
                if (
                    ActionMap[i].ActiveDevice==VIRTUAL_DEV_ID&&
                    ActionMap[i].ActiveType==uievent->VirtualEvent
                    )
                {
                    /* Active the callback */
                    CallbackFcnPtr=ActionMap[i].ActiveFcn;
                    (*CallbackFcnPtr)(ACNodePtr,FcnRegPtr,localposition,uievent);
                }
                break;
            case DEV_MOUSE_ID:
                if (
                    ActionMap[i].ActiveDevice==DEV_MOUSE_ID&&
                    ActionMap[i].ActiveType==MouseEventPtr->mosevent
                    )
                {
                    /* Active the callback */
                    CallbackFcnPtr=ActionMap[i].ActiveFcn;
                    (*CallbackFcnPtr)(ACNodePtr,FcnRegPtr,localposition,uievent);
                }
                break;
            case DEV_KEYBOARD_ID:
                if (ActionMap[i].ActiveDevice==DEV_KEYBOARD_ID&&
                    ActionMap[i].ActiveType==uievent->KBEvent
                    )
                {
                    /* Active the callback */
                    CallbackFcnPtr=ActionMap[i].ActiveFcn;
                    (*CallbackFcnPtr)(ACNodePtr,FcnRegPtr,localposition,uievent);
                }
                break;

            default:
                printf("Unsupported device!\n");
                break;
            }

        }
    }
}


/*
*	Active Specific Function Region
*/
void activesfr(
               ABSTRACT_COMPONENT_NODE_PTR ACNPtr,
               FUNCTION_REGION_NODE_PTR			FcnRegPtr,
               COORDINATE_PTR				localposition,
               UI_EVENT_PTR				uievent
               )
{
    int i;
    ACTION_MAP_PTR ActionMapPtr;
    MOUSE_EVENT_PTR MouseEventPtr;
    void(*CallbackFcnPtr)(void*,void*,void*,void*);

    if (
        FcnRegPtr==NULL		/* NULL Function region pointer */
        || FcnRegPtr->Enable==False	/* invalid function region */
        || FcnRegPtr->ActionFcnMap==NULL	/* No Action map */
        || isInRegion(&(FcnRegPtr->GeometricRegion),localposition)==False	/* out of region */
        )
    {
        return;
    }
    ActionMapPtr=FcnRegPtr->ActionFcnMap;
    MouseEventPtr=uievent->MouseEvent;
    for (i=0;i<FcnRegPtr->ActiveMapAmount;i++)
    {
        switch(uievent->ActiveDevice)
        {
        case VIRTUAL_DEV_ID:
            if (ActionMapPtr[i].ActiveDevice==VIRTUAL_DEV_ID&&
                uievent->VirtualEvent==ActionMapPtr[i].ActiveType
                )
            {
                CallbackFcnPtr=ActionMapPtr[i].ActiveFcn;
                (*CallbackFcnPtr)(ACNPtr,FcnRegPtr,localposition,uievent);
            }
            break;
        case DEV_MOUSE_ID:
            if (
                ActionMapPtr[i].ActiveDevice==DEV_MOUSE_ID&&
                ActionMapPtr[i].ActiveType==MouseEventPtr->mosevent
                )
            {
                CallbackFcnPtr=ActionMapPtr[i].ActiveFcn;
                (*CallbackFcnPtr)(ACNPtr,FcnRegPtr,localposition,uievent);
            }
            break;
        case DEV_KEYBOARD_ID:
            if (
                ActionMapPtr[i].ActiveDevice==DEV_KEYBOARD_ID&&
                ActionMapPtr[i].ActiveType==uievent->KBEvent
                )
            {
                CallbackFcnPtr=ActionMapPtr[i].ActiveFcn;
                (*CallbackFcnPtr)(ACNPtr,FcnRegPtr,localposition,uievent);
            }
            break;
        default:
            printf("Unsupported device!\n");
            break;
        }
    }
}
