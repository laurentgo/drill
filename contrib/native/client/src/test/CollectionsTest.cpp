#include <iostream>
#include <string>
#include <vector>

#include <boost/assign.hpp>
#include <boost/shared_ptr.hpp>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "drill/collections.hpp"

namespace {
template<typename T>
class DrillVectorIteratorImpl: public Drill::impl::DrillIteratorImpl<T> {
    public:
        typedef Drill::impl::DrillIteratorImpl<T> supertype;

        DrillVectorIteratorImpl(const typename std::vector<T>::iterator& it): m_it(it) {};

        T& operator*() const { return m_it.operator *();}
        T* operator->() const { return m_it.operator->(); }

        operator typename Drill::impl::DrillIteratorImpl<const T>::iterator_ptr() const { return typename Drill::impl::DrillIteratorImpl<const T>::iterator_ptr(new DrillVectorIteratorImpl<const T>(m_it)); }

        DrillVectorIteratorImpl& operator++() { 
            m_it++; return *this;
        }

        bool operator==(const supertype& x) const {
            const DrillVectorIteratorImpl<T>& other(dynamic_cast<const DrillVectorIteratorImpl<T>&>(x));
            return m_it == other.m_it;
        }

        bool operator!=(const Drill::impl::DrillIteratorImpl<T>& x) const { return !(*this==x); }

    private:
        typename std::vector<T>::iterator m_it;
};

template<typename T>
class DrillVectorImpl: public Drill::impl::DrillCollectionImpl<T> {
    public:
        DrillVectorImpl() {}
        DrillVectorImpl(const std::vector<T>& v): m_vector(v) {};

        boost::shared_ptr<Drill::impl::DrillIteratorImpl<T> > begin() { return boost::shared_ptr<Drill::impl::DrillIteratorImpl<T> >(new DrillVectorIteratorImpl<T>(m_vector.begin()));}
        boost::shared_ptr<Drill::impl::DrillIteratorImpl<const T> > begin() const { return boost::shared_ptr<Drill::impl::DrillIteratorImpl<const T> >(new DrillVectorIteratorImpl<const T>(m_vector.begin()));}
        boost::shared_ptr<Drill::impl::DrillIteratorImpl<T> > end() { return boost::shared_ptr<Drill::impl::DrillIteratorImpl<T> >(new DrillVectorIteratorImpl<T>(m_vector.end()));}
        boost::shared_ptr<Drill::impl::DrillIteratorImpl<const T> > end() const { return boost::shared_ptr<Drill::impl::DrillIteratorImpl<const T> >(new DrillVectorIteratorImpl<const T>(m_vector.end()));}

    private:
        std::vector<T> m_vector;
};

template<typename T>
class DrillVector: public Drill::DrillCollection<T> {
    public:
        DrillVector(const std::vector<T>& v): Drill::DrillCollection<T>(boost::shared_ptr<Drill::impl::DrillCollectionImpl<T> >(new DrillVectorImpl<T>(v))) {}
};
} // anonymous namespace
class CollectionsTest: public CppUnit::TestFixture {
public:
    CollectionsTest() {}

    CPPUNIT_TEST_SUITE( CollectionsTest );
    CPPUNIT_TEST( testDrillVector );
    CPPUNIT_TEST( testConstDrillVector );
    CPPUNIT_TEST_SUITE_END();

    void testDrillVector() {
        std::vector<std::string> v = boost::assign::list_of("foo")("bar");

        DrillVector<std::string> drillCollection(v);
        std::vector<std::string> result;
        for(DrillVector<std::string>::const_iterator it = drillCollection.begin(); it != drillCollection.end(); ++it) {
            result.push_back(*it);
        }

        CPPUNIT_ASSERT(result == v); 
    }

    void testConstDrillVector() {
            std::vector<std::string> v = boost::assign::list_of("foo")("bar");

            const DrillVector<std::string> drillCollection(v);
            std::vector<std::string> result;
            for(DrillVector<std::string>::const_iterator it = drillCollection.begin(); it != drillCollection.end(); ++it) {
                result.push_back(*it);
            }

            CPPUNIT_ASSERT(result == v);
        }
};

CPPUNIT_TEST_SUITE_REGISTRATION( CollectionsTest );
