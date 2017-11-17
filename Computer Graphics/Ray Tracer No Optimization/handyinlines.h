#ifndef HANDY_INLINES_H
#define HANDY_INLINES_H

template <class M>
inline const M& min(const M& a, const M& b)
{
    return (a>b?b:a);
}

inline std::string operator+( std::string& st, unsigned int x)
{
    std::stringstream oss;
    oss<<st<<x;
    return oss.str();
}

#endif
