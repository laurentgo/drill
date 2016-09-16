#include <iostream>
#include <string>
#include <vector>

#include <boost/assign.hpp>
#include <boost/shared_ptr.hpp>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "drill/collections.hpp"
#include "collectionsImpl.hpp"

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

struct SimpleInterface {
	virtual ~SimpleInterface() {}
	virtual const std::string& foo() const = 0;
	virtual std::string bar() = 0;
};

class SimpleImplementation: public SimpleInterface {
public:
	SimpleImplementation(const std::string& foo, const std::string& bar): m_foo(foo), m_bar(bar) {}

	const std::string& foo() const { return m_foo; }
	std::string bar() { return m_bar; }

private:
	std::string m_foo;
	std::string m_bar;
};

} // anonymous namespace

class CollectionsTest: public CppUnit::TestFixture {
public:
    CollectionsTest() {}

    CPPUNIT_TEST_SUITE( CollectionsTest );
    CPPUNIT_TEST( testSimpleCollection );
    CPPUNIT_TEST( testSimpleConstCollection );
    CPPUNIT_TEST( testDrillVectorConstIterator );
    CPPUNIT_TEST( testDrillVectorIterator );
    CPPUNIT_TEST( testDrillVectorConstPointer );
    CPPUNIT_TEST_SUITE_END();

    void testSimpleCollection() {
    	// basic test/proof of concept for collections.hpp

        std::vector<std::string> v = boost::assign::list_of("foo")("bar");

        DrillVector<std::string> drillCollection(v);
        std::vector<std::string> result;
        for(DrillVector<std::string>::const_iterator it = drillCollection.begin(); it != drillCollection.end(); ++it) {
            result.push_back(*it);
        }

        CPPUNIT_ASSERT(result == v); 
    }

    void testSimpleConstCollection() {
            std::vector<std::string> v = boost::assign::list_of("foo")("bar");

            const DrillVector<std::string> drillCollection(v);
            std::vector<std::string> result;
            for(DrillVector<std::string>::const_iterator it = drillCollection.begin(); it != drillCollection.end(); ++it) {
                result.push_back(*it);
            }

            CPPUNIT_ASSERT(result == v);
        }

    void testDrillVectorConstIterator() {
    	typedef Drill::DrillVector<SimpleInterface, SimpleImplementation> SimpleInterfaceVector;
    	SimpleInterfaceVector v;

    	v.push_back(SimpleImplementation("foo1", "bar1"));
    	v.push_back(SimpleImplementation("foo2", "bar2"));

    	std::vector<std::string> resultFoo;
    	SimpleInterfaceVector::const_iterator it(v.begin());
    	for(; it != v.end(); ++it) {
    		resultFoo.push_back(it->foo());
    		// const-correctness: The following line does not compile if uncommented!
    		// resultBar.push_back(it->bar());
    	}

    	std::vector<std::string> expectedFoo = boost::assign::list_of("foo1")("foo2");

    	CPPUNIT_ASSERT(resultFoo == expectedFoo);
    }

    void testDrillVectorIterator() {
    	typedef Drill::DrillVector<SimpleInterface, SimpleImplementation> SimpleInterfaceVector;
    	SimpleInterfaceVector v;

    	v.push_back(SimpleImplementation("foo1", "bar1"));
    	v.push_back(SimpleImplementation("foo2", "bar2"));

    	std::vector<std::string> resultFoo;
    	std::vector<std::string> resultBar;
    	SimpleInterfaceVector::iterator it;
    	for(it = v.begin(); it != v.end(); ++it) {
    		resultFoo.push_back(it->foo());
    		resultBar.push_back(it->bar());
    	}

    	std::vector<std::string> expectedFoo = boost::assign::list_of("foo1")("foo2");
    	std::vector<std::string> expectedBar = boost::assign::list_of("bar1")("bar2");

    	CPPUNIT_ASSERT(resultFoo == expectedFoo);
    	CPPUNIT_ASSERT(resultBar == expectedBar);
    }

    // Check some const-correctness issues
    // by getting iterators of a const collection
    void testDrillVectorConstPointer() {
    	typedef Drill::DrillVector<SimpleInterface, SimpleImplementation> SimpleInterfaceVector;
    	boost::shared_ptr<SimpleInterfaceVector> v(new SimpleInterfaceVector);

    	const SimpleInterfaceVector* vv(v.get());

    	v->push_back(SimpleImplementation("foo1", "bar1"));
    	v->push_back(SimpleImplementation("foo2", "bar2"));

    	std::vector<std::string> resultFoo;
    	std::vector<std::string> resultBar;
    	SimpleInterfaceVector::const_iterator it;
    	for(it = vv->begin(); it != vv->end(); ++it) {
    		resultFoo.push_back(it->foo());
    	}

    	std::vector<std::string> expectedFoo = boost::assign::list_of("foo1")("foo2");

    	CPPUNIT_ASSERT(resultFoo == expectedFoo);
    }

};

CPPUNIT_TEST_SUITE_REGISTRATION( CollectionsTest );
