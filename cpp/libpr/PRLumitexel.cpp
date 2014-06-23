/*
Tao Du
taodu@stanford.edu
May 14, 2014
*/

#include "PRLumitexel.h"

PRLumitexel::PRLumitexel()
{
	m_brdfSamples.clear();
}

PRLumitexel::~PRLumitexel()
{
	int length = (int)m_brdfSamples.size();
	for (int i = 0; i < length; i++)
		delete m_brdfSamples[i];
	m_brdfSamples.clear();
}

void PRLumitexel::addBrdfSample(PRBrdfSample *sample)
{
	//	don't add null pointer!
	if (sample)
		m_brdfSamples.push_back(sample);
}

int PRLumitexel::getBrdfSampleNum()
{
	return (int)m_brdfSamples.size();
}

double PRLumitexel::getTheta(int i)
{
	int num = (int)m_brdfSamples.size();
	assert(i >= 0 && i < num);
	return m_brdfSamples[i]->getTheta();
}

double PRLumitexel::getCosTheta(int i)
{
	int num = (int)m_brdfSamples.size();
	assert(i >= 0 && i < num);
	return m_brdfSamples[i]->getCosTheta();
}

PRVector3 PRLumitexel::getRadiance(int i)
{
	int num = (int)m_brdfSamples.size();
	assert(i >= 0 && i < num);
	return m_brdfSamples[i]->getRadiance();
}

double PRLumitexel::getRadiance(int i, int channel)
{
	int num = (int)m_brdfSamples.size();
	assert(i >= 0 && i < num);
	return m_brdfSamples[i]->getRadiance(channel);
}

PRVector3 PRLumitexel::getAlbedo()
{
	PRVector3 albedo;
	int num = (int)m_brdfSamples.size();
	assert(num > 0);
	int count = 0;
	for (int i = 0; i < num; i++)
	{
		double theta = m_brdfSamples[i]->getTheta();
		if (theta > 0.3 && theta < 0.7)
		{
			albedo += m_brdfSamples[i]->getRadiance();
			count++;
		}
	}
	if (count > 0)
		return albedo / count;
	else
		return PRVector3::inf();
}

/*
PRVector3 PRLumitexel::getMedianRadiance()
{
	int num = (int)m_brdfSamples.size();
	assert(num > 0);
	std::vector<double> samples;
	PRVector3 medianRadiance;
	for (int channel = 0; channel < 3; channel++)
	{
		samples.clear();
		//	add samples for this specific channel
		for (int i = 0; i < num; i++)
		{
			samples.push_back(m_brdfSamples[i]->getRadiance(channel));
		}
		//	sort samples
		std::sort(samples.begin(), samples.end());
		//	find the median
		medianRadiance[channel] = samples.at(num / 2);
	}
	return medianRadiance;
}
*/