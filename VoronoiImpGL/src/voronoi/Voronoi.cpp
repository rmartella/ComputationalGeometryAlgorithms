#include "Voronoi.h"
#include <iostream>
#include <algorithm>
#include <set>
#include <cmath>

using namespace vor;

Voronoi::Voronoi() {
	edges = 0;
}

Edges * Voronoi::GetEdges(Vertices * v, int w, int h) {
	places = v;
	width = w;
	height = h;
	root = 0;

	if (!edges)
		edges = new Edges();
	else {
		for (Vertices::iterator i = points.begin(); i != points.end(); ++i)
			delete (*i);
		for (Edges::iterator i = edges->begin(); i != edges->end(); ++i)
			delete (*i);
		points.clear();
		edges->clear();
	}

	for (Vertices::iterator i = places->begin(); i != places->end(); ++i) {
		queue.push(new VEvent(*i, true));
	}

	VEvent * e;
	while (!queue.empty()) {
		e = queue.top();
		queue.pop();
		ly = e->point->y;
		if (deleted.find(e) != deleted.end()) {
			delete (e);
			deleted.erase(e);
			continue;
		}
		if (e->pe)
			InsertParabola(e->point);
		else
			RemoveParabola(e);
		delete (e);
	}

	FinishEdge(root);

	for (Edges::iterator i = edges->begin(); i != edges->end(); ++i) {
		if ((*i)->neighbour) {
			(*i)->start = (*i)->neighbour->end;
			delete (*i)->neighbour;
		}
	}

	return edges;
}

void Voronoi::InsertParabola(VPoint * p) {
	if (!root) {
		root = new VParabola(p);
		return;
	}

	if (root->isLeaf && root->site->y - p->y < 0.01) // degenerovan� p��pad - ob� spodn� m�sta ve stejn� v�ce
			{
		VPoint * fp = root->site;
		root->isLeaf = false;
		root->SetLeft(new VParabola(fp));
		root->SetRight(new VParabola(p));
		VPoint * s = new VPoint((p->x + fp->x) / 2, height); // za��tek hrany uprost�ed m�st
		points.push_back(s);
		if (p->x > fp->x)
			root->edge = new VEdge(s, fp, p); // rozhodnu, kter� vlevo, kter� vpravo
		else
			root->edge = new VEdge(s, p, fp);
		edges->push_back(root->edge);
		return;
	}

	VParabola * par = GetParabolaByX(p->x);

	if (par->cEvent) {
		deleted.insert(par->cEvent);
		par->cEvent = 0;
	}

	VPoint * start = new VPoint(p->x, GetY(par->site, p->x));
	points.push_back(start);

	VEdge * el = new VEdge(start, par->site, p);
	VEdge * er = new VEdge(start, p, par->site);

	el->neighbour = er;
	edges->push_back(el);

	// p�estavuju strom .. vkl�d�m novou parabolu
	par->edge = er;
	par->isLeaf = false;

	VParabola * p0 = new VParabola(par->site);
	VParabola * p1 = new VParabola(p);
	VParabola * p2 = new VParabola(par->site);

	par->SetRight(p2);
	par->SetLeft(new VParabola());
	par->Left()->edge = el;

	par->Left()->SetLeft(p0);
	par->Left()->SetRight(p1);

	CheckCircle(p0);
	CheckCircle(p2);
}

void Voronoi::RemoveParabola(VEvent * e) {
	bool degeneryCase = false;
	VParabola * p1 = e->arch;

	VParabola * xl = VParabola::GetLeftParent(p1);
	VParabola * xr = VParabola::GetRightParent(p1);

	VParabola * p0 = VParabola::GetLeftChild(xl);
	VParabola * p2 = VParabola::GetRightChild(xr);

	if (p0 == p2)
		std::cout << "chyba - prav� a lev� parabola m� stejn� ohnisko!\n";

	if ((p0->cEvent && p0->cEvent->point->x == e->point->x
			&& p0->cEvent->point->y == e->point->y)
			|| (p2->cEvent && p2->cEvent->point->x == e->point->x
					&& e->point->y == p2->cEvent->point->y)) {
		//degeneryCase = true;
	}

	if (degeneryCase) {
		VPoint * p = new VPoint(e->point->x, GetY(p1->site, e->point->x));
		points.push_back(p);

		xl->edge->end = p;
		xr->edge->end = p;
		VParabola * gparent = p1->parent->parent;
		if (p1->parent->Left() == p1) {
			if (gparent->Left() == p1->parent)
				gparent->SetLeft(p1->parent->Right());
			if (gparent->Right() == p1->parent)
				gparent->SetRight(p1->parent->Right());
		} else {
			if (gparent->Left() == p1->parent)
				gparent->SetLeft(p1->parent->Left());
			if (gparent->Right() == p1->parent)
				gparent->SetRight(p1->parent->Left());
		}

		delete p1->parent;
		delete p1;
	} else {
		if (p0->cEvent) {
			deleted.insert(p0->cEvent);
			p0->cEvent = 0;
		}
		if (p2->cEvent) {
			deleted.insert(p2->cEvent);
			p2->cEvent = 0;
		}

		VPoint * p = new VPoint(e->point->x, GetY(p1->site, e->point->x));
		points.push_back(p);

		xl->edge->end = p;
		xr->edge->end = p;

		VParabola * higher;
		VParabola * par = p1;
		while (par != root) {
			par = par->parent;
			if (par == xl)
				higher = xl;
			if (par == xr)
				higher = xr;
		}
		higher->edge = new VEdge(p, p0->site, p2->site);
		edges->push_back(higher->edge);

		VParabola * gparent = p1->parent->parent;
		if (p1->parent->Left() == p1) {
			if (gparent->Left() == p1->parent)
				gparent->SetLeft(p1->parent->Right());
			if (gparent->Right() == p1->parent)
				gparent->SetRight(p1->parent->Right());
		} else {
			if (gparent->Left() == p1->parent)
				gparent->SetLeft(p1->parent->Left());
			if (gparent->Right() == p1->parent)
				gparent->SetRight(p1->parent->Left());
		}

		delete p1->parent;
		delete p1;

		CheckCircle(p0);
		CheckCircle(p2);
	}

}

void Voronoi::FinishEdge(VParabola * n) {
	if (n->isLeaf) {
		delete n;
		return;
	}

	VPoint * end;
	if (std::isinf(n->edge->f)) {
		if (n->edge->direction->x > 0.0)
			end = new VPoint(n->edge->start->x, width);
		else
			end = new VPoint(n->edge->start->x, -10);
	} else {
		double mx;
		if (n->edge->direction->x > 0.0)
			mx = std::max(width, n->edge->start->x + 10);
		else
			mx = std::min(0.0, n->edge->start->x - 10);
		end = new VPoint(mx, mx * n->edge->f + n->edge->g);
	}

	n->edge->end = end;
	points.push_back(end);

	FinishEdge(n->Left());
	FinishEdge(n->Right());
	delete n;
}

double Voronoi::GetXOfEdge(VParabola * par, double y) {
	VParabola * left = VParabola::GetLeftChild(par);
	VParabola * right = VParabola::GetRightChild(par);

	VPoint * p = left->site;
	VPoint * r = right->site;

	if (p->y - y == 0)
		return p->x;
	if (r->y - y == 0)
		return r->x;

	double dp;
	dp = 2.0 * (p->y - y);
	double a1 = 1.0 / dp;
	double b1 = -2.0 * p->x / dp;
	double c1 = y + dp / 4 + p->x * p->x / dp;

	dp = 2.0 * (r->y - y);
	double a2 = 1.0 / dp;
	double b2 = -2.0 * r->x / dp;
	double c2 = ly + dp / 4 + r->x * r->x / dp;

	double a = a1 - a2;
	if (a != 0) {
		double b = b1 - b2;
		double c = c1 - c2;

		double disc = b * b - 4 * a * c;
		double x1 = (-b + std::sqrt(disc)) / (2 * a);
		double x2 = (-b - std::sqrt(disc)) / (2 * a);
		double ry;
		if (p->y < r->y)
			ry = std::max(x1, x2);
		else
			ry = std::min(x1, x2);
		return ry;
	} else
		return (p->x + r->x) / 2.0f;
	//return par->site->x;

}

VParabola * Voronoi::GetParabolaByX(double xx) {
	VParabola * par = root;
	double x = 0.0;

	while (!par->isLeaf) // projdu stromem dokud nenaraz�m na vhodn� list
	{
		x = GetXOfEdge(par, ly);
		if (x > xx)
			par = par->Left();
		else
			par = par->Right();
	}
	return par;
}

double Voronoi::GetY(VPoint * p, double x) // ohnisko, x-sou�adnice
		{
	double dp;
	if (p->y - ly == 0)
		dp = 0.0001;
	else
		dp = 2 * (p->y - ly);
	double a1 = 1 / dp;
	double b1 = -2 * p->x / dp;
	double c1 = ly + dp / 4 + p->x * p->x / dp;

	return (a1 * x * x + b1 * x + c1);
}

void Voronoi::CheckCircle(VParabola * b) {
	VParabola * lp = VParabola::GetLeftParent(b);
	VParabola * rp = VParabola::GetRightParent(b);

	VParabola * a = VParabola::GetLeftChild(lp);
	VParabola * c = VParabola::GetRightChild(rp);

	if (!a || !c || a->site == c->site)
		return;

	VPoint * s = 0;
	s = GetEdgeIntersection(lp->edge, rp->edge);
	if (s == 0)
		return;

	double dx = a->site->x - s->x;
	double dy = a->site->y - s->y;

	double d = std::sqrt((dx * dx) + (dy * dy));

	if (s->y - d >= ly) {
		return;
	}

	VEvent * e = new VEvent(new VPoint(s->x, s->y - d), false);
	points.push_back(e->point);
	b->cEvent = e;
	e->arch = b;
	queue.push(e);
}

VPoint * Voronoi::GetEdgeIntersection(VEdge * a, VEdge * b) {

	double x, y;

	if ((std::isinf(a->f) && !std::isinf(b->f)) || (std::isinf(b->f) && !std::isinf(a->f))) {
		if (std::isinf(a->f)) {
			x = (a->left->x + a->right->x) / 2.0;
			y = b->f * x + b->g;
		} else {
			x = (b->left->x + b->right->x) / 2.0;
			y = a->f * x + a->g;
		}
	} else if (!std::isinf(a->f) && !std::isinf(b->f)) {
		if (a->f - b->f == 0.0)
			std::cout << "par" << std::endl;
		x = (b->g - a->g) / (a->f - b->f);
		y = a->f * x + a->g;
	}

	/*float A1 = a->p1->y - a->p0->y;
	 float A2 = b->p1->y - b->p0->y;
	 float B1 = a->p0->x - a->p1->x;
	 float B2 = b->p0->x - b->p1->x;

	 if (A2 == B2 && A2 == 0) {
	 x = b->p0->x;
	 y = b->p0->y;
	 } else if (A1 == B1 && A1 == 0) {
	 x = a->p0->x;
	 y = a->p0->y;
	 } else {
	 float C1 = A1 * a->p0->x + B1 * a->p0->y;
	 float C2 = A2 * b->p0->x + B2 * b->p0->y;
	 double det = A1 * B2 - A2 * B1;
	 x = (B2 * C1 - B1 * C2) / det;
	 y = (A1 * C2 - A2 * C1) / det;
	 }*/

	if ((x - a->start->x) * a->direction->x < 0)
		return 0;
	if ((y - a->start->y) * a->direction->y < 0)
		return 0;

	if ((x - b->start->x) * b->direction->x < 0)
		return 0;
	if ((y - b->start->y) * b->direction->y < 0)
		return 0;

	/*if (a->direction->x != 0 && (x - a->start->x) / a->direction->x < 0)
	 return 0;
	 if (a->direction->y != 0 && (y - a->start->y) / a->direction->y < 0)
	 return 0;

	 if (b->direction->x != 0 && (x - b->start->x) / b->direction->x < 0)
	 return 0;
	 if (b->direction->y != 0 && (y - b->start->y) / b->direction->y < 0)
	 return 0;*/

	/*if ((x - a->start->x) / a->direction->x < 0)
	 return 0;
	 if ((y - a->start->y) / a->direction->y < 0)
	 return 0;

	 if ((x - b->start->x) / b->direction->x < 0)
	 return 0;
	 if ((y - b->start->y) / b->direction->y < 0)
	 return 0;*/

	VPoint * p = new VPoint(x, y);
	points.push_back(p);
	return p;
}

