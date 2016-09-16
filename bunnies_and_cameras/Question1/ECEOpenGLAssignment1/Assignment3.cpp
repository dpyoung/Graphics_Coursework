#include <GL/glut.h>
#include <iostream>
//#include <graphics.h>
#include <conio.h>
#include <math.h>


/*************************************************************************///---------------------------------  Edge  ------------------------------///*************************************************************************/
class Edge
	{
	public:
		int yUpper;

		float xIntersect;
		float dxPerScan;

		Edge *next;
	};

	/*************************************************************************///--------------------------  PointCoordinates  -------------------------///*************************************************************************/
	class PointCoordinates
	{
	public:
		float x;
		float y;

		PointCoordinates()
		{
			x = 0;
			y = 0;
		}
	};

	/*************************************************************************///---------------------------  LineCoordinates  -------------------------///*************************************************************************/
	class LineCoordinates
	{
	public:
		float x_1;
		float y_1;
		float x_2;
		float y_2;

		LineCoordinates()
		{
			x_1 = 0;
			y_1 = 0;
			x_2 = 0;
			y_2 = 0;
		}

		LineCoordinates(const float x1, const float y1,
			const float x2, const float y2)
		{
			x_1 = x1;
			y_1 = y1;
			x_2 = x2;
			y_2 = y2;
		}
	};

	/*************************************************************************//*************************************************************************///-----------------------  Function Prototypes  -------------------------///*************************************************************************//*************************************************************************/
	void show_screen( );

	void Fill_polygon(const int, const int[]);

	void insertEdge(Edge *, Edge *);
	void makeEdgeRec(const PointCoordinates, const PointCoordinates,
		const int, Edge *, Edge *[]);
	void buildEdgeList(const int, const PointCoordinates[], Edge *[]);
	void buildActiveList(const int, Edge *, Edge *[]);
	void fillScan(const int, const Edge *);
	void deleteAfter(Edge[]);
	void updateActiveList(const int, Edge[]);
	void resortActiveList(Edge[]);

	const int yNext(const int, const int, const PointCoordinates[]);

	void Polygon(const int, const int[]);
	void Line(const int, const int, const int, const int);


	void draw_pixel(int x, int y)
	{
		glColor3f(0.0, 1.0, 1.0);
		glPointSize(1.0);
		glBegin(GL_POINTS);
		glVertex2i(x, y);
		glEnd();
	}

	int main()
	{

		//initgraph(&driver, &mode, "..\\Bgi");

		show_screen();

		int n = 10;
		int polygon_points[20] = { 220,340 , 220,220 , 250,170 , 270,200 ,
			300,140 , 320,240 , 320,290 , 420,220 ,
			420,340 , 220,340 };

		setcolor(15);
		Polygon(10, polygon_points);

		Fill_polygon(n, polygon_points, 9);

		getch();
		return 0;
	}

	/*
		Fill dirty work
	*/
	void Fill_polygon(const int n,const int ppts[])
	{
		Edge *edges[480];
		Edge *active;

		PointCoordinates *pts = new PointCoordinates[n];

		for (int count_1 = 0; count_1<n; count_1++)
		{
			pts[count_1].x = (ppts[(count_1 * 2)]);
			pts[count_1].y = (ppts[((count_1 * 2) + 1)]);
		}

		for (int count_2 = 0; count_2<640; count_2++)
		{
			edges[count_2] = new Edge;
			edges[count_2]->next = NULL;
		}

		buildEdgeList(n, pts, edges);

		active = new Edge;
		active->next = NULL;

		for (int count_3 = 0; count_3<480; count_3++)
		{
			buildActiveList(count_3, active, edges);

			if (active->next)
			{
				fillScan(count_3, active);
				updateActiveList(count_3, active);
				resortActiveList(active);
			}
		}

		Polygon(n, ppts);

		delete pts;
	}

	/*
		
	*/
	const int yNext(const int k,const int cnt,const PointCoordinates pts[])
	{
		int j;

		if ((k + 1)>(cnt - 1))
			j = 0;

		else
			j = (k + 1);

		while (pts[k].y == pts[j].y)
		{
			if ((j + 1)>(cnt - 1))
				j = 0;

			else
				j++;
		}

		return (pts[j].y);
	}

	/*
		insert edges into a list
	*/
	void insertEdge(Edge *list, Edge *edge)
	{
		Edge *p;
		Edge *q = list;

		p = q->next;

		while (p != NULL)
		{
			if (edge->xIntersect<p->xIntersect)
				p = NULL;

			else
			{
				q = p;
				p = p->next;
			}
		}

		edge->next = q->next;
		q->next = edge;
	}

	/*
		record the edge in the edges array
	*/
	void makeEdgeRec(const PointCoordinates lower, const PointCoordinates upper,
	const int yComp, Edge *edge, Edge *edges[])
	{
		edge->dxPerScan = ((upper.x - lower.x) / (upper.y - lower.y));
		edge->xIntersect = lower.x;

		if (upper.y<yComp)
			edge->yUpper = (upper.y - 1);

		else
			edge->yUpper = upper.y;

		insertEdge(edges[lower.y], edge);
	}

	/*
		Build the edge list structure
	*/
	void buildEdgeList(const int cnt,const PointCoordinates pts[],Edge *edges[])
	{
		Edge *edge;
		PointCoordinates v1;
		PointCoordinates v2;

		int yPrev = (pts[cnt - 2].y);

		v1.x = pts[cnt - 1].x;
		v1.y = pts[cnt - 1].y;

		for (int count = 0; count<cnt; count++)
		{
			v2 = pts[count];

			if (v1.y != v2.y)
			{
				edge = new Edge;

				if (v1.y<v2.y)
					makeEdgeRec(v1, v2, yNext(count, cnt, pts), edge, edges);

				else
					makeEdgeRec(v2, v1, yPrev, edge, edges);
			}

			yPrev = v1.y;
			v1 = v2;
		}
	}

	/*
		Build the active list from the list of edges.
	*/
	void buildActiveList(const int scan, Edge *active, Edge *edges[])
	{
		Edge *p;
		Edge *q;

		p = edges[scan]->next;

		while (p)
		{
			q = p->next;

			insertEdge(active, p);

			p = q;
		}
	}

	/*
		Fillscan, for kicks.
	*/
	void fillScan(const int scan,const Edge *active)
	{
		Edge *p1;
		Edge *p2;

		p1 = active->next;

		while (p1)
		{
			p2 = p1->next;

			for (int count = p1->xIntersect; count <= p2->xIntersect; count++)
				draw_pixel(count, scan);

			p1 = p2->next;
		}
	}

	/*
		Delete after, as is needed for the fillscan
	*/
	void deleteAfter(Edge * q)
	{
		Edge *p = q->next;

		q->next = p->next;

		delete p;
	}

	/*
		Update the active list...?
	*/
	void updateActiveList(const int scan,Edge *active)
	{
		Edge *q = active;
		Edge *p = active->next;

		while (p)
		{
			if (scan >= p->yUpper)
			{
				p = p->next;

				deleteAfter(q);
			}

			else
			{
				p->xIntersect = (p->xIntersect + p->dxPerScan);
				q = p;
				p = p->next;
			}
		}
	}

	/*
		Resort, like a bucket
	*/
	void resortActiveList(Edge *active)
	{
		Edge *q;
		Edge *p = active->next;

		active->next = NULL;

		while (p)
		{
			q = p->next;

			insertEdge(active, p);

			p = q;
		}
	}

	/*
		class defining such polygons
	*/
	void Polygon(const int n, const int coordinates[])
	{
		if (n >= 2)
		{
			Line(coordinates[0], coordinates[1],
				coordinates[2], coordinates[3]);

			for (int count = 1; count<(n - 1); count++)
				Line(coordinates[(count * 2)], coordinates[((count * 2) + 1)],
					coordinates[((count + 1) * 2)],
					coordinates[(((count + 1) * 2) + 1)]);
		}
	}

	/*
		Defines an edge/line
	*/
	void Line(const int x_1,const int y_1,const int x_2,const int y_2)
	{
		//int color = getcolor();

		int x1 = x_1;
		int y1 = y_1;

		int x2 = x_2;
		int y2 = y_2;

		if (x_1>x_2)
		{
			x1 = x_2;
			y1 = y_2;

			x2 = x_1;
			y2 = y_1;
		}

		int dx = abs(x2 - x1);
		int dy = abs(y2 - y1);
		int inc_dec = ((y2 >= y1) ? 1 : -1);

		if (dx>dy)
		{
			int two_dy = (2 * dy);
			int two_dy_dx = (2 * (dy - dx));
			int p = ((2 * dy) - dx);

			int x = x1;
			int y = y1;

			draw_pixel(x, y);

			while (x<x2)
			{
				x++;

				if (p<0)
					p += two_dy;

				else
				{
					y += inc_dec;
					p += two_dy_dx;
				}

				draw_pixel(x, y);
			}
		}

		else
		{
			int two_dx = (2 * dx);
			int two_dx_dy = (2 * (dx - dy));
			int p = ((2 * dx) - dy);

			int x = x1;
			int y = y1;

			draw_pixel(x, y);

			while (y != y2)
			{
				y += inc_dec;

				if (p<0)
					p += two_dx;

				else
				{
					x++;
					p += two_dx_dy;
				}

				draw_pixel(x, y);
			}
		}
	}

	/*************************************************************************///--------------------------  show_screen( )  ---------------------------///*************************************************************************/
	void show_screen( )
	{
		setfillstyle(1, 1);
		bar(178, 26, 450, 38);

		settextstyle(0, 0, 1);
		setcolor(15);
		outtextxy(5, 5, "******************************************************************************");
		outtextxy(5, 17, "*-**************************************************************************-*");
		outtextxy(5, 29, "*-------------------                                     --------------------*");
		outtextxy(5, 41, "*-**************************************************************************-*");
		outtextxy(5, 53, "*-**************************************************************************-*");

		setcolor(11);
		outtextxy(185, 29, "Scan Line Polygon Fill Algorithm");

		setcolor(15);

		for (int count = 0; count <= 30; count++)
			outtextxy(5, (65 + (count * 12)), "*-*                                                                        *-*");

		outtextxy(5, 438, "*-**************************************************************************-*");
		outtextxy(5, 450, "*-------------------------                          -------------------------*");
		outtextxy(5, 462, "******************************************************************************");

		setcolor(12);
		outtextxy(229, 450, "Press any Key to exit.");
	}

	/*source heavily adapted from http://www.dailyfreecode.com/code/fill-polygon-scan-line-polygon-fill-673.aspx */