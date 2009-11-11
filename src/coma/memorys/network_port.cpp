/*
mgsim: Microgrid Simulator
Copyright (C) 2006,2007,2008,2009  The Microgrid Project.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
*/
#include "network_port.h"
using namespace MemSim;


void Network_port::ConnectNodes()
{
	int size = m_vecLpNode.size();
	if (size <= 1)
	{
		cerr << ERR_HEAD_OUTPUT << "Network with less than 2 nodes!" << endl;
		exit(-1);
	}
	
	Network_Node* pNodePre = m_vecLpNode.at(0);
	Network_Node* pNode;
	for (int i=1;i<size;i++)
	{
		pNode = m_vecLpNode.at(i);
		pNodePre->m_fifoNetOut(pNode->m_fifoNetIn);
        clog << "connecting " << pNode->name() << endl;

		pNodePre = pNode;
	}

	pNode->m_fifoNetOut((m_vecLpNode.at(0))->m_fifoNetIn);


}

