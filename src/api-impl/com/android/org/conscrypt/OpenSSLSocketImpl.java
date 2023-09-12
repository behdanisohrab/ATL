package com.android.org.conscrypt;

import javax.net.ssl.SSLSocket;

public abstract class OpenSSLSocketImpl extends SSLSocket {

	public abstract void setUseSessionTickets(boolean useSessionTickets);

	public abstract void setHostname(String hostName);

	public abstract byte[] getAlpnSelectedProtocol();

	public abstract void setAlpnProtocols​(byte[] alpnProtos);
}
