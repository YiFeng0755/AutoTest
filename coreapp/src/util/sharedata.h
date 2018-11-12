#ifndef SHAREDATA_H
#define SHAREDATA_H

class PrivateData;
class MyData
{
public:
    MyData();
    MyData(const MyData& data);
    MyData& operator=(MyData& data);

private:
    PrivateData *d_ptr;
};


class PrivateData
{
public:
    PrivateData()
        :m_ref(0),
          m_d(0.0),
          m_t(0)
    {}
    ~PrivateData(){
        if (m_t){
            delete  m_t;
            m_t = 0;
        }
    }

    int m_ref;
    double m_d;
    char *m_t;
};



#endif // SHAREDATA_H
