// introduced as of CNet v4.21 - 06-Aug-96 to perform background
// test and transform of file items.

#ifndef CNET_MANAGER
	#define CNET_MANAGER



#define CNETFILEBOX "cnet:MANAGER"

// one of these exists for each file Text/Transform or DCC request/transfer/chat.
struct FileTask_Info
	{
	struct Node fti_Node;

	int type;	// which type FTI_FILE	= fti_file
					//            FTI_DCC	= fti_dccstruct

	union
		{
		struct
			{
			struct Node dcc_Node;		 // DOS node - ln_Name points to nickname

			LONG	UserID;					 // owner's CNet IDNumber (UserData.IDNumber)
			char	ID[20];					 // a unique ASCII-coded ID for this DCC instance.
												 // ID is used to reference/lookup/FindName() DCC nodes
												 //	in the main list (myz->MPE->DCCList) maintained
												 // by file-task for DCC file transfers.
												 // dcc_Node.ln_Name points to this ID.
												 // myp->MPE->dccsem is used to lock the MainPort DCC
												 // list before altering/walking it.

			char Type[20];					 // DCC type (eg. CHAT, SEND, GET)
			char arg[128];					 // DCC Arg (eg. filename or "chat")
			char senderaddr[20];			 // string integer representation of address (LOCAL address if send; remote address if GET)
			LONG port;						 // remote port number
			char nickname[30];			 // remote user nick
			ULONG size;						 // total filesize
			LONG flags;						 // see DR_FLAG_* below
			UBYTE status;					 // see DCC_STATUS_* below

			UBYTE TypeF;					 // see DCC_TYPEF_* below

			ULONG lastval;              // private internal use
			LONG txfertime;             // private internal use

			LONG CPScount;              // current CPS

			struct DCCFile	FileInfo;	 // file information
			struct SignalSemaphore sem; // semaphore locked before removal takes place
			struct Process *DCCProc;	 // pointer to File-task DCC child process
			char asciiaddr[16];			 // ascii representation of remote address (for DCC GET)
			} dccstruct;

		struct
			{
			LONG id;					// serial id of the user who posted/uploaded the file
		                     	// account number not used to avoid awarding creds to the wrong user

			char UniqueBase[75];	// physical location (GO keyword) of file to be tested or transformed
								// this is only used if PhysNum is -1

			short PhysNum;			// (2-Feb-1997) -  Physical subboard NUMBER of the base where the file exists
			ULONG ItemNum;			// unique serial number of message
			char FileName[72];	// name of file to test or transform. FILENAME only, no path
			short ArcNum;			// number of archiver to use in test/tx..
			LONG ProcessType;		// See TX_* bits below
			LONG CurrentType;		// for display of "info" in file-task - current processing state (TX_* bits) mutually exclusive
			UBYTE NotifyUser;		// 0=do not notify user
		                        // 1=notify user - if online, send SYSTEM OLM, if offline,
		                        //   sends mail to user regarding successful or failed
										//   test or transform and includes the test results if a test.
										// this field is currently ALWAYS 1 (show results)

			LONG checksum;	// this must be 0xAAAA or file will not be processed!
			} txfile;
		} fti_u;
	} __attribute__((packed));


// defines for FileInfo_Type.type
#define FTI_FILE 0x01
#define FTI_DCC  0x02


#define DR_FLAG_RESUME		0x01

#define DCC_STATUS_WAIT			0	// 0 = waiting for user to accept/deny
#define DCC_STATUS_OPEN			1	// 1 = accepted/open
#define DCC_STATUS_STALLED		2	// 2 = file stalled
#define DCC_STATUS_CLOSED		3	// 3 = DCC aborted
#define DCC_STATUS_COMPLETE	4	// 4 = successful completion
#define DCC_STATUS_FAIL			5	// 5 = connect failed
#define DCC_STATUS_POSTSEND	6	// 6 = file-task posted SEND request
#define DCC_STATUS_IP_STALL   7	// 7 = at IP lookup stage (during SEND)
											//     or connect() stage (during get)

#define DCC_TYPEF_CHAT       0      // for CHAT
#define DCC_TYPEF_INFILEREQ  1      // for INCOMING file (remote SEND, local GET)
#define DCC_TYPEF_OUTFILEREQ 2      // for OUTGOING file (remote GET, local SEND)
#define DCC_TYPEF_INCHATREQ  3      // for INCOMING chat request
#define DCC_TYPEF_OUTCHATREQ 4      // for OUTGOING chat request   
#define DCC_TYPEF_ABORT      (1<<7) // ABORTED because file-task quit if (TypeF & DCC_TYPE_ABORT)


// defines for ProcessType of union member struct fti_txFile
#define TX_TEST      0x01
#define TX_TRANSFORM 0x02


// Extended Node used to track active test/transform processes.
// myp->MPE->TxProcessList contains a list of these nodes, when active.
struct TxProcess
	{
	struct Node tp_Node;				// Node for Exec list linkage
	struct Process *process;		// Exec Process node
	char name[16];						// process name
	char txfile[256];					// manager filename (CNET:MANAGER/xxxxxxxxxxxx)
	struct FileTask_Info *tx_fti; // information about the file being tested and the
											// user who initiated the test.
	} __attribute__((packed));



// to have cnet's manager perform a test or transform, file a single FILLED in
// TXFile structure to a file (with any UNIQUE filename) to CNETFILEBOX
// (defined above).
//
// When the file is closed, cnet-manager will be notified that a new file
// is to be tested or transformed and the manager will then take action.

#endif
