#include"ARC.h"

ARC * Node::getArcWithNode(Node * nd)
{
	for (auto & arc1 : _connectArcSet)
	{	
		pair<Node *, Node*> nodePair = arc1->getNodePair();
		if (nodePair.first == nd || nodePair.second == nd)
		{
			return arc1;
		}
	}
	return NULL;
 }