#include <algorithm>
#include <iterator>
#include "random.h"
#include "network.h"

typedef std::multimap<size_t,size_t>::const_iterator cit; 
typedef std::multimap<size_t,size_t>::iterator it;

void Network::resize(const size_t& n)
{ 
	if(values.size()==0)
	{ 
		values=std::vector<double> (n,0);
		RNG.normal(values);
	}
	if(n<values.size()){
		for (size_t i(values.size()); i>=n ;--i) { values.pop_back();}
	}else{
		if(n>values.size())
		{
			
			for (size_t i(values.size()); i<n ; ++i)
			{
				values.push_back(RNG.normal());
			}
		}
	}
}
	
bool Network::add_link(const size_t& a, const size_t& b)
{	
	if (a>=values.size()or b>=values.size() or a<0 or b<0  or a==b)
	{ return false;} 
	
	
	for(it at=links.begin(); at!=links.end(); ++at)
	{ 
		if(((at->first==b) and (at->second==a) )){ return false; }
		 //or ((at->first==a) and (at->second==b)))
		
	}
	links.insert(std::make_pair(a,b));
	links.insert(std::make_pair(b,a));
	return true;
}

size_t Network::random_connect(const double& mean)
{ 
	links.clear();
	std::vector<int> tab(values.size());

	
	
	int somme=0;
	for (size_t l(0); l<values.size() ; ++l)
	{		
		size_t deg=RNG.poisson(mean);

		for(size_t k(0); k<deg ;++k)
		{	RNG.uniform_int(tab);
			if(add_link(tab[k],l)) { somme+=1;} 
			else{ if(deg<tab.size()){ ++deg; } }
		}
	}
	return somme;
}
		
size_t Network::set_values(const std::vector<double>& tab)
{
	for(auto& element : values) element=0; 
	if(tab.size()==values.size())
	{ values=tab; 
	  return values.size();
	}
	
	if(tab.size()>values.size())
	 {
		 for(size_t i(0);i<values.size(); ++i)
		 { values[i]=tab[i];}
		 return values.size();
	 }
	
	if(tab.size()<values.size()) 
	{ 
		for(size_t i(0);i<tab.size(); ++i)
		{ values[i]=tab[i];}
		return tab.size();
	}
	
}
	
size_t Network::size() const
{ return values.size(); }

size_t Network::degree(const size_t &_n) const
{ if(_n>=0 and _n<values.size()) return links.count(_n);
  else return 0;
}

double Network::value(const size_t &_n) const
{  if(_n>=0 and _n<values.size()) return values[_n]; 
   else return 0;
}

std::vector<double> Network::sorted_values() const
{	
	std::vector<double> tab(values);
	std::sort (tab.begin(), tab.end());
	std::reverse (tab.begin(), tab.end());
	
	return tab;
}
	
std::vector<size_t> Network::neighbors(const size_t& n) const
{
	std::vector<size_t> neig;
	if(n>values.size() or n<=0) return neig;
	for (cit at=links.begin(); at!= links.end(); ++at)
	{
		if(at->first==n){ 
		neig.push_back(at->second);
		}
	}
	
	return neig;
}
