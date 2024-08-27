// Open a COM port.
int OpenCom(int port, DCB *dcb, int timeout, int ask);
// Close a COM port.
int CloseCom(int port);
// Read n bytes from a COM port to buf.
int ReadCom(int port, unsigned char *buf, int *nbytes);
// Write n bytes to a COM port from buf.
int WriteCom(int port, unsigned char *buf, int nbytes);
// Flush port's buffers.
void FlushCom(int port);
// Reset a COM port.
void ResetCom(int port);
// Configure a port.
int SetComState(int port, DCB *dcb);
// Retrieve modem control-register value.
int GetComStatus(int port);
