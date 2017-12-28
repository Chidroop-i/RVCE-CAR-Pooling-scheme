#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <fstream>

using namespace std;

class Point
{
private:
	int id_point, id_cluster;
	vector<double> values;
	int total_values;
	string name;

public:
	Point(int id_point, vector<double>& values, string name = "")
	{
		this->id_point = id_point;
		total_values = values.size();

		for(int i = 0; i < total_values; i++)
			this->values.push_back(values[i]);

		this->name = name;
		id_cluster = -1;
	}

	int getID()
	{
		return id_point;
	}

	void setCluster(int id_cluster)
	{
		this->id_cluster = id_cluster;
	}

	int getCluster()
	{
		return id_cluster;
	}

	double getValue(int index)
	{
		return values[index];
	}

	int getTotalValues()
	{
		return total_values;
	}

	void addValue(double value)
	{
		values.push_back(value);
	}

	string getName()
	{
		return name;
	}
};

class Cluster
{
private:
	int id_cluster;
	vector<double> central_values;
	vector<Point> points;

public:
	Cluster(int id_cluster, Point point)
	{
		this->id_cluster = id_cluster;

		int total_values = point.getTotalValues();

		for(int i = 0; i < total_values; i++)
			central_values.push_back(point.getValue(i));

		points.push_back(point);
	}

	void addPoint(Point point)
	{
		points.push_back(point);
	}

	bool removePoint(int id_point)
	{
		int total_points = points.size();

		for(int i = 0; i < total_points; i++)
		{
			if(points[i].getID() == id_point)
			{
				points.erase(points.begin() + i);
				return true;
			}
		}
		return false;
	}

	double getCentralValue(int index)
	{
		return central_values[index];
	}

	void setCentralValue(int index, double value)
	{
		central_values[index] = value;
	}

	Point getPoint(int index)
	{
		return points[index];
	}

	int getTotalPoints()
	{
		return points.size();
	}

	int getID()
	{
		return id_cluster;
	}
};

class KMeans
{
private:
	int K; 
	int total_values, total_points, max_iterations;
	vector<Cluster> clusters;

	
	int getIDNearestCenter(Point point)
	{
		double sum = 0.0, min_dist;
		int id_cluster_center = 0;

		for(int i = 0; i < total_values; i++)
		{
			sum += pow(clusters[0].getCentralValue(i) -
					   point.getValue(i), 2.0);
		}

		min_dist = sqrt(sum);

		for(int i = 1; i < K; i++)
		{
			double dist;
			sum = 0.0;

			for(int j = 0; j < total_values; j++)
			{
				sum += pow(clusters[i].getCentralValue(j) -
						   point.getValue(j), 2.0);
			}

			dist = sqrt(sum);

			if(dist < min_dist)
			{
				min_dist = dist;
				id_cluster_center = i;
			}
		}

		return id_cluster_center;
	}

public:
	KMeans(int K, int total_points, int total_values, int max_iterations)
	{
		this->K = K;
		this->total_points = total_points;
		this->total_values = total_values;
		this->max_iterations = max_iterations;
	}

	void run(vector<Point> & points)
	{
		if(K > total_points)
			return;

		vector<int> prohibited_indexes;

		// choose K distinct values for the centers of the clusters
		for(int i = 0; i < K; i++)
		{
			while(true)
			{
				int index_point = rand() % total_points;

				if(find(prohibited_indexes.begin(), prohibited_indexes.end(),
						index_point) == prohibited_indexes.end())
				{
					prohibited_indexes.push_back(index_point);
					points[index_point].setCluster(i);
					Cluster cluster(i, points[index_point]);
					clusters.push_back(cluster);
					break;
				}
			}
		}

		int iter = 1;

		while(true)
		{
			bool done = true;

			// associates each point to the nearest center
			for(int i = 0; i < total_points; i++)
			{
				int id_old_cluster = points[i].getCluster();
				int id_nearest_center = getIDNearestCenter(points[i]);

				if(id_old_cluster != id_nearest_center)
				{
					if(id_old_cluster != -1)
						clusters[id_old_cluster].removePoint(points[i].getID());

					points[i].setCluster(id_nearest_center);
					clusters[id_nearest_center].addPoint(points[i]);
					done = false;
				}
			}

			// recalculating the center of each cluster
			for(int i = 0; i < K; i++)
			{
				for(int j = 0; j < total_values; j++)
				{
					int total_points_cluster = clusters[i].getTotalPoints();
					double sum = 0.0;

					if(total_points_cluster > 0)
					{
						for(int p = 0; p < total_points_cluster; p++)
							sum += clusters[i].getPoint(p).getValue(j);
						clusters[i].setCentralValue(j, sum / total_points_cluster);
					}
				}
			}

			if(done == true || iter >= max_iterations)
			{
				cout << "Break in iteration " << iter << "\n\n";
				break;
			}

			iter++;
		}
		
		
 ofstream ofile; // declaring an object of class ofstream
   ofile.open("Output.html"); // open "file.txt" for writing data
   /* write to a file */
   ofile<<"<!DOCTYPE html> <html > <head> <meta charset=\"UTF-8\"> <title>Cluster Hover</title> <link rel='stylesheet prefetch' href='https://fonts.googleapis.com/css?family=Source+Sans+Pro:200,400,700,400italic,700italic'> <style> /* NOTE: The styles were added inline because Prefixfree needs access to your styles and they must be inlined if they are on local disk! */ /*----------------------------------------------- $ Main -----------------------------------------------*/ html, body { padding: 0; margin: 0; } html, body, input, button, textarea { font: 100%/1.4 \"Source Sans Pro\", \"Open Sans\", \"Helvetica Neue\", Helvetica, Arial, sans-serif; } a, button { transition: color 0.15s, background-color 0.15s; } /* General, main styles */ .wrapper { min-width: 290px; max-width: 800px; padding-left: 1em; padding-right: 1em; margin-left: auto; margin-right: auto; } /* Helpers */ .text-giga { line-height: 1; font-size: 24px; font-weight: 300; } .inline { display: inline; } .pad-top { padding-top: 1em; } .space-out { margin: 100px; } .space-bottom { margin-bottom: 1em; } /*----------------------------------------------- $ Buttons -----------------------------------------------*/ .btn { width: 35px; height: 35px; border: 2px solid #333; border-radius: 50%; outline: 0; display: inline-block; background-color: rgba(255,255,255,0.35); color: #333; font-weight: bold; text-decoration: none; line-height: 30px; text-align: center; vertical-align: middle; box-sizing: border-box; } .btn:hover { background-color: #4285f4; color: white; } .btn:active { background-color: #4285f4; transition: none; } /*----------------------------------------------- $ Cluster Menu -----------------------------------------------*/ .menu-cluster-holder { display: inline-block; position: relative; } .menu-cluster { width: 50px; height: 50px; padding: 0; margin: 0; position: absolute; top: 0; left: 0; opacity: 0; transition: opacity 0.25s 0.4s; } .menu-cluster > li { padding: 0; margin: 0; position: absolute; top: 0; left: 0; list-style: none; transform: translate(0%) translateY(0%); transition: transform 0.25s 0.4s, opacity 0.25s 0.4s; } .menu-cluster-toggler { position: absolute z-index: 1; } /* .is-hovered is for the automatic JS demo, * otherwise it is not needed. */ .menu-cluster-toggler:hover + .menu-cluster, .menu-cluster:hover, .menu-cluster-toggler:hover + .menu-cluster > li, .menu-cluster:hover > li, .menu-cluster-toggler.is-hovered + .menu-cluster, .menu-cluster.is-hovered, .menu-cluster-toggler.is-hovered + .menu-cluster > li, .menu-cluster.is-hovered > li { opacity: 1; transition-delay: 0s; } .menu-cluster-toggler:hover + .menu-cluster > li, .menu-cluster:hover > li, .menu-cluster-toggler.is-hovered + .menu-cluster > li, .menu-cluster.is-hovered > li { transform: translateX(0%) translateY(-125%); } .menu-cluster-toggler:hover + .menu-cluster > li:nth-child(2), .menu-cluster:hover > li:nth-child(2), .menu-cluster-toggler.is-hovered + .menu-cluster > li:nth-child(2), .menu-cluster.is-hovered > li:nth-child(2) { transform: translateX(115%) translateY(-70%); } .menu-cluster-toggler:hover + .menu-cluster > li:nth-child(3), .menu-cluster:hover > li:nth-child(3), .menu-cluster-toggler.is-hovered + .menu-cluster > li:nth-child(3), .menu-cluster.is-hovered > li:nth-child(3) { transform: translateX(115%) translateY(60%); } .menu-cluster-toggler:hover + .menu-cluster > li:nth-child(4), .menu-cluster:hover > li:nth-child(4), .menu-cluster-toggler.is-hovered + .menu-cluster > li:nth-child(4), .menu-cluster.is-hovered > li:nth-child(4) { transform: translateX(0%) translateY(125%); } .menu-cluster-toggler:hover + .menu-cluster > li:nth-child(5), .menu-cluster:hover > li:nth-child(5), .menu-cluster-toggler.is-hovered + .menu-cluster > li:nth-child(5), .menu-cluster.is-hovered > li:nth-child(5) { transform: translateX(-115%) translateY(60%); } .menu-cluster-toggler:hover + .menu-cluster > li:nth-child(6), .menu-cluster:hover > li:nth-child(6), .menu-cluster-toggler.is-hovered + .menu-cluster > li:nth-child(6), .menu-cluster.is-hovered > li:nth-child(6) { transform: translateX(-115%) translateY(-70%); } </style> <script src=\"https://cdnjs.cloudflare.com/ajax/libs/prefixfree/1.0.7/prefixfree.min.js\"></script> </head> <body background=\"bg.png\"> <div class=\"main-header pad-top wrapper\"> </div>"<<endl;
  
		// shows elements of clusters
		for(int i = 0; i < K; i++)
		{
			int total_points_cluster =  clusters[i].getTotalPoints();
				ofile<<"<div class=\"main-content space-top wrapper\" role=\"main\"> <div class=\"menu-cluster-holder space-out\" style=\"left:"<<(clusters[i].getCentralValue(0))*4<<"px;top:"<<-(clusters[i].getCentralValue(1))*4<<"px;\">"<<endl;
				ofile<<"<button type=\"button\" class=\"btn space-bottom menu-cluster-toggler\" id=\"menuClusterToggler\">"<<i<<"</button>"<<endl;
				ofile<<"<ul class=\"menu-cluster\">"<<endl;
			cout << "Cluster " << clusters[i].getID() + 1 << endl;
			for(int j = 0; j < total_points_cluster; j++)
			{	
				cout << "Point " << clusters[i].getPoint(j).getID() + 1 << ": ";
				for(int p = 0; p < total_values; p++)
					cout << clusters[i].getPoint(j).getValue(p) << " ";

				string point_name = clusters[i].getPoint(j).getName();

				if(point_name != "")
					cout << "- " << point_name;

				cout << endl;
				ofile<<"<li><a href=\"#a\" class=\"btn\">"<<point_name<<"</a></li>"<<endl;
			}
			
			cout << "Cluster values: ";

			for(int j = 0; j < total_values; j++)
				cout << clusters[i].getCentralValue(j) << " ";

			cout << "\n\n";
			ofile<<"</ul></div>"<<endl;
		}
		
	ofile << "<script src='http://cdnjs.cloudflare.com/ajax/libs/jquery/2.1.3/jquery.min.js'></script> <script src=\"js/index.js\"></script> </body> </html>" << endl;
   
   cout << "Data written to file" << endl;
   ofile.close(); 
	}
};

int main(int argc, char *argv[])
{
	srand (time(NULL));

	int total_points, total_values, K, max_iterations, has_name;

	cin >> total_points >> total_values >> K >> max_iterations >> has_name;

	vector<Point> points;
	string point_name;

	for(int i = 0; i < total_points; i++)
	{
		vector<double> values;

		for(int j = 0; j < total_values; j++)
		{
			double value;
			cin >> value;
			values.push_back(value);
		}

		if(has_name)
		{
			cin >> point_name;
			Point p(i, values, point_name);
			points.push_back(p);
		}
		else
		{
			Point p(i, values);
			points.push_back(p);
		}
	}

  
  
	KMeans kmeans(K, total_points, total_values, max_iterations);
	kmeans.run(points);
	

	return 0;
}
