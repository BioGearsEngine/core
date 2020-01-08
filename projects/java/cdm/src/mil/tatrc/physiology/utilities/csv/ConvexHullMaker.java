/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/
package mil.tatrc.physiology.utilities.csv;

import java.util.ArrayList;
import java.util.List;
import mil.tatrc.physiology.utilities.Log;

public class ConvexHullMaker
{
	// This is a java program to find a points in convex hull using quick hull
	// method
	// source: Alexander Hrishov's website
	// URL: http://www.ahristov.com/tutorial/geometry-games/convex-hull.html
	public static void main(String args[])
	{
		
	}
	
	public class Point
	{
		double x;
		double y;
		
		public Point(Double X, Double Y)
    {
	    x = X;
	    y= Y;
    }
	}
	
	public List<List<Double>> make(List<Double> xVals, List<Double> yVals)
	{
		return pointsToLists(quickHull(listsToPoints(xVals, yVals)));
	}
	
	public ArrayList<Point> listsToPoints(List<Double> xVals, List<Double> yVals)
	{
		if(xVals.size() != yVals.size())
		{
			Log.error("The number of X values is different than the number of Y values");
			return null;
		}
		
		ArrayList<Point> points = new ArrayList<Point>();
		
		for(int i = 0; i < xVals.size(); i++)
		{
			Point p = new Point(xVals.get(i), yVals.get(i));
			points.add(p);
		}
		
		return points;
	}
	
	public List<List<Double>> pointsToLists(ArrayList<Point> points)
	{		
		List<List<Double>> lists = new ArrayList<List<Double>>();
		List<Double> xVals = new ArrayList<Double>();
		List<Double> yVals = new ArrayList<Double>();
		
		for(int i = 0; i < points.size(); i++)
		{
			xVals.add(points.get(i).x);
			yVals.add(points.get(i).y);
		}
		
		lists.add(xVals);
		lists.add(yVals);
		
		return lists;
	}
	
	public ArrayList<Point> quickHull(ArrayList<Point> points)
	{
		ArrayList<Point> convexHull = new ArrayList<Point>();
		if (points.size() < 3)
			return (ArrayList) points.clone();

		int minPoint = -1, maxPoint = -1;
		double minX = Double.MAX_VALUE;
		double maxX = -Double.MAX_VALUE;

		for (int i = 0; i < points.size(); i++)
		{
			if (points.get(i).x < minX)
			{
				minX = points.get(i).x;
				minPoint = i;
			}

			if (points.get(i).x > maxX)
			{
				maxX = points.get(i).x;
				maxPoint = i;
			}
		}
		
		if(maxPoint == -1 || minPoint == -1)
			Log.error("Couldn't determine either min or max point on convex hull.");
		
		Point A = points.get(minPoint);
		Point B = points.get(maxPoint);
		
		convexHull.add(A);
		convexHull.add(B);
		
		points.remove(A);
		points.remove(B);
		
		ArrayList<Point> leftSet = new ArrayList<Point>();
		ArrayList<Point> rightSet = new ArrayList<Point>();
		
		for (int i = 0; i < points.size(); i++)
		{
			Point p = points.get(i);
			
			if (pointLocation(A, B, p) == -1)
				leftSet.add(p);
			else if (pointLocation(A, B, p) == 1)
				rightSet.add(p);
		}
		
		hullSet(A, B, rightSet, convexHull);
		hullSet(B, A, leftSet, convexHull);
		
		return convexHull;
	}

	public double distance(Point A, Point B, Point C)
	{
		double ABx = B.x - A.x;
		double ABy = B.y - A.y;
		double num = ABx * (A.y - C.y) - ABy * (A.x - C.x);
		
		if (num < 0)
			num = -num;
		
		return num;
	}

	public void hullSet(Point A, Point B, ArrayList<Point> set,	ArrayList<Point> hull)
	{
		int insertPosition = hull.indexOf(B);
		
		if (set.size() == 0)
			return;
		
		if (set.size() == 1)
		{
			Point p = set.get(0);
			set.remove(p);
			hull.add(insertPosition, p);
			return;
		}
		
		double dist = Double.MIN_VALUE;
		int furthestPoint = -1;
		
		for (int i = 0; i < set.size(); i++)
		{
			Point p = set.get(i);
			double distance = distance(A, B, p);
			if (distance > dist)
			{
				dist = distance;
				furthestPoint = i;
			}
		}
		
		Point P = set.get(furthestPoint);
		set.remove(furthestPoint);
		hull.add(insertPosition, P);

		// Determine who's to the left of AP
		ArrayList<Point> leftSetAP = new ArrayList<Point>();
		for (int i = 0; i < set.size(); i++)
		{
			Point M = set.get(i);
			if (pointLocation(A, P, M) == 1)
			{
				leftSetAP.add(M);
			}
		}

		// Determine who's to the left of PB
		ArrayList<Point> leftSetPB = new ArrayList<Point>();
		for (int i = 0; i < set.size(); i++)
		{
			Point M = set.get(i);
			if (pointLocation(P, B, M) == 1)
			{
				leftSetPB.add(M);
			}
		}
		hullSet(A, P, leftSetAP, hull);
		hullSet(P, B, leftSetPB, hull);
	}

	public double pointLocation(Point A, Point B, Point P)
	{
		double cp1 = (B.x - A.x) * (P.y - A.y) - (B.y - A.y) * (P.x - A.x);
		
		if (cp1 > 0)
			return 1;
		else if (cp1 == 0)
			return 0;
		else
			return -1;
	}

}
