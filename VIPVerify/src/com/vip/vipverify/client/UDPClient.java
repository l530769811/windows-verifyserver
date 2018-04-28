package com.vip.vipverify.client;

import java.io.IOException;
import java.io.Serializable;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

import com.vip.vipverify.net.SocketReceiveListeningChannelList;
import com.vip.vipverify.operator.DoOperator;
import com.vip.vipverify.operator.UdpSendDoOperator;
import com.vip.vipverify.thread.OperateWakeThread;
import com.vip.vipverify.thread.WakeThread;

public class UDPClient implements Runnable, Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	private DatagramSocket m_client = null;

	private int m_nport = 0;
	private String m_ip = null;
	private Thread rev_thread = null;
	private boolean isRevThreadOutside = false;

	private WakeThread m_threadSend = null;
	private boolean bisOutsideSendThread = false;
	private SocketReceiveListeningChannelList listener = null;

	private boolean bexit = false;

	private String tmp_ip = "";

	public UDPClient(int m_nPort, Thread receive_thread, WakeThread threadSend,
			SocketReceiveListeningChannelList listener) {
		this.m_nport = m_nPort;
		this.m_threadSend = threadSend;
		this.rev_thread = receive_thread;
		if(this.rev_thread!=null) {
			this.isRevThreadOutside = true;
		}
		this.listener = listener;
		if (this.m_threadSend != null)
			bisOutsideSendThread = true;
		
	}

	public UDPClient(String ip, int m_nPort, WakeThread threadSend, SocketReceiveListeningChannelList listener) {
		this.m_nport = m_nPort;
		this.m_ip = ip;
		this.m_threadSend = threadSend;
		this.listener = listener;
		if (this.m_threadSend != null)
			bisOutsideSendThread = true;
	}

	@Override
	public void run() {
		// TODO Auto-generated method stub

		while (true) {
			if (bexit)
				break;

			byte[] rev_byte = new byte[1024];
			InetAddress rev_addr = null;
			try {
				rev_addr = InetAddress.getByName("255.255.255.255");
			} catch (UnknownHostException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				continue;
			}
			DatagramPacket rev_pack = new DatagramPacket(rev_byte, rev_byte.length, rev_addr, m_nport);
			try {
				m_client.receive(rev_pack);
				if (listener != null) {
					listener.receive_data(rev_pack.getData(), rev_pack.getLength());
				}

			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

		}

	}

	public boolean UDPClientInit() {

		if (m_client == null || m_client.isClosed()) {
			try {
				SocketAddress addr = null;

				try {
					addr = new InetSocketAddress(m_ip, m_nport);
				} catch (Exception e) {
					// TODO: handle exception
				}

				if (addr != null) {
					m_client = new DatagramSocket(addr);
				} else if (m_nport != -1 && addr == null) {
					m_client = new DatagramSocket(m_nport);
				} else {
					m_client = new DatagramSocket();
				}

				if (rev_thread == null && isRevThreadOutside == false) {
					rev_thread = new Thread(this);
					rev_thread.start();
				}

				return true;

			} catch (SocketException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();

			}
		}
		return false;
	}

	public void UDPClientColse() {
		bexit = true;

		try {
			if (rev_thread != null && isRevThreadOutside == false) {
				rev_thread.join();
			}
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		if (m_client != null && m_client.isClosed() == false)
			m_client.close();

		if (bisOutsideSendThread == false) {
			if (m_threadSend != null) {
				m_threadSend.exit();
			}
		}
	}

	public int SendDataInQueue(String strIp, int port, byte[] bDataPack, int nSize) {
		int nret = -1;

		if (m_threadSend == null || (m_threadSend.isAlive() == false && bisOutsideSendThread == false)) {
			m_threadSend = new OperateWakeThread();
			m_threadSend.start();
		}

		DoOperator operate = new UdpSendDoOperator(m_client, strIp, m_nport, bDataPack, nSize,
				!tmp_ip.equalsIgnoreCase(strIp));
		if (m_threadSend.postOperate(operate) == true)
			nret = nSize;

		return nret;
	}

	public int SendDataPack(String strIp, int port, byte[] bDataPack, int nSize) {

		InetAddress send_addr = null;

		try {

			send_addr = InetAddress.getByName(strIp);

		} catch (UnknownHostException e1) {

			// TODO Auto-generated catch block

			e1.printStackTrace();
			return -1;
		}

		int len = bDataPack.length;
		DatagramPacket send_pack = new DatagramPacket(bDataPack, len, send_addr, port);

		int ret = -1;
		try {

			m_client.send(send_pack);
			ret = nSize;

		} catch (IOException e) {

			// TODO Auto-generated catch block
			e.printStackTrace();

			ret = -1;
		}

		return ret;
	}
}
