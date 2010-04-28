#include	"testHeader.h"

union val 
{
  int				i_val;
  long				l_val;
  struct linger		linger_val;
  struct timeval	timeval_val;
} val;

static char	*sock_str_flag(union val *, int);
static char	*sock_str_int(union val *, int);
static char	*sock_str_linger(union val *, int);
static char	*sock_str_timeval(union val *, int);

#undef IPV6
#undef IPPROTO_SCTP
struct sock_opts 
{
  const char	   *opt_str;
  int			    opt_level;
  int			    opt_name;
  char				*(*opt_val_str)(union val *, int);
} sock_opts[] = {
	{ "SO_BROADCAST",		SOL_SOCKET,	SO_BROADCAST,	sock_str_flag },
	{ "SO_DEBUG",			SOL_SOCKET,	SO_DEBUG,		sock_str_flag },
	{ "SO_DONTROUTE",		SOL_SOCKET,	SO_DONTROUTE,	sock_str_flag },
	{ "SO_ERROR",			SOL_SOCKET,	SO_ERROR,		sock_str_int },
	{ "SO_KEEPALIVE",		SOL_SOCKET,	SO_KEEPALIVE,	sock_str_flag },
	{ "SO_LINGER",			SOL_SOCKET,	SO_LINGER,		sock_str_linger },
	{ "SO_OOBINLINE",		SOL_SOCKET,	SO_OOBINLINE,	sock_str_flag },
	{ "SO_RCVBUF",			SOL_SOCKET,	SO_RCVBUF,		sock_str_int },
	{ "SO_SNDBUF",			SOL_SOCKET,	SO_SNDBUF,		sock_str_int },
	{ "SO_RCVLOWAT",		SOL_SOCKET,	SO_RCVLOWAT,	sock_str_int },
	{ "SO_SNDLOWAT",		SOL_SOCKET,	SO_SNDLOWAT,	sock_str_int },
	{ "SO_RCVTIMEO",		SOL_SOCKET,	SO_RCVTIMEO,	sock_str_timeval },
	{ "SO_SNDTIMEO",		SOL_SOCKET,	SO_SNDTIMEO,	sock_str_timeval },
	{ "SO_REUSEADDR",		SOL_SOCKET,	SO_REUSEADDR,	sock_str_flag },
#ifdef	SO_REUSEPORT
	{ "SO_REUSEPORT",		SOL_SOCKET,	SO_REUSEPORT,	sock_str_flag },
#else
	{ "SO_REUSEPORT",		0,			0,				AII_NULL },
#endif
	{ "SO_TYPE",			SOL_SOCKET,	SO_TYPE,		sock_str_int },
#ifdef	SO_USELOOPBACK
	{ "SO_USELOOPBACK",		SOL_SOCKET,	SO_USELOOPBACK,	sock_str_flag },
#else
	{ "SO_USELOOPBACK",		0,			0,				AII_NULL },
#endif
	{ "IP_TOS",				IPPROTO_IP,	IP_TOS,			sock_str_int },
	{ "IP_TTL",				IPPROTO_IP,	IP_TTL,			sock_str_int },
#ifdef	IPV6
	{ "IPV6_DONTFRAG",		IPPROTO_IPV6,IPV6_DONTFRAG,	sock_str_flag },
	{ "IPV6_UNICAST_HOPS",	IPPROTO_IPV6,IPV6_UNICAST_HOPS,sock_str_int },
	{ "IPV6_V6ONLY",		IPPROTO_IPV6,IPV6_V6ONLY,	sock_str_flag },
#else
	{ "IPV6_DONTFRAG",		0,			0,				AII_NULL },
	{ "IPV6_UNICAST_HOPS",	0,			0,				AII_NULL },
	{ "IPV6_V6ONLY",		0,			0,				AII_NULL },
#endif
	{ "TCP_MAXSEG",			IPPROTO_TCP,TCP_MAXSEG,		sock_str_int },
	{ "TCP_NODELAY",		IPPROTO_TCP,TCP_NODELAY,	sock_str_flag },
#ifdef	IPPROTO_SCTP
	{ "SCTP_AUTOCLOSE",		IPPROTO_SCTP,SCTP_AUTOCLOSE,sock_str_int },
	{ "SCTP_MAXBURST",		IPPROTO_SCTP,SCTP_MAXBURST,	sock_str_int },
	{ "SCTP_MAXSEG",		IPPROTO_SCTP,SCTP_MAXSEG,	sock_str_int },
	{ "SCTP_NODELAY",		IPPROTO_SCTP,SCTP_NODELAY,	sock_str_flag },
#else
	{ "SCTP_AUTOCLOSE",		0,			0,				AII_NULL },
	{ "SCTP_MAXBURST",		0,			0,				AII_NULL },
	{ "SCTP_MAXSEG",		0,			0,				AII_NULL },
	{ "SCTP_NODELAY",		0,			0,				AII_NULL },
#endif
	{ AII_NULL,					0,			0,				AII_NULL }
};

int testDefSockopts(const char* recvBuf, char* sendBuf)
{
	int					fd = -1;
	socklen_t			len;
	struct sock_opts	*ptr;

	for (ptr = sock_opts; ptr->opt_str != AII_NULL; ptr++) 
	{
		printf("%s: ", ptr->opt_str);
		if (ptr->opt_val_str == AII_NULL)
		{
			printf("(undefined)\n");
		}
		else 
		{
			switch(ptr->opt_level) 
			{
				case SOL_SOCKET:
				case IPPROTO_IP:
				case IPPROTO_TCP:
					fd = socket(AF_INET, SOCK_STREAM, 0);
					break;
				#ifdef	IPV6
				case IPPROTO_IPV6:
					fd = socket(AF_INET6, SOCK_STREAM, 0);
					break;
				#endif
				#ifdef	IPPROTO_SCTP
				case IPPROTO_SCTP:
					fd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);
					break;
				#endif
				default:
					printf("Can't create fd for level %d\n", ptr->opt_level);
			}

			len = sizeof(val);
			if (getsockopt(fd, ptr->opt_level, ptr->opt_name, &val, &len) == -1) 
			{
				printf("getsockopt error\n");
			} else 
			{
				printf("default = %s\n", (*ptr->opt_val_str)(&val, len));
			}
			close(fd);
		}
	}
	
	return 0;
}

static char	strres[128];
static char	*sock_str_flag(union val *ptr, int len)
{
	if (len != sizeof(int))
	{
		snprintf(strres, sizeof(strres), "size (%d) not sizeof(int)", len);
	}
	else
	{
		snprintf(strres, sizeof(strres), "%s", (ptr->i_val == 0) ? "off" : "on");
	}
	return(strres);
}

static char	*sock_str_int(union val *ptr, int len)
{
	if (len != sizeof(int))
	{
		snprintf(strres, sizeof(strres), "size (%d) not sizeof(int)", len);
	}
	else
	{
		snprintf(strres, sizeof(strres), "%d", ptr->i_val);
	}
	return(strres);
}

static char	*sock_str_linger(union val *ptr, int len)
{
	struct linger	*lptr = &ptr->linger_val;

	if (len != sizeof(struct linger))
	{
		snprintf(strres, sizeof(strres), "size (%d) not sizeof(struct linger)", len);
	}
	else
	{
		snprintf(strres, sizeof(strres), "l_onoff = %d, l_linger = %d", lptr->l_onoff, lptr->l_linger);
	}
	return(strres);
}

static char	*sock_str_timeval(union val *ptr, int len)
{
	struct timeval	*tvptr = &ptr->timeval_val;

	if (len != sizeof(struct timeval))
	{
		snprintf(strres, sizeof(strres), "size (%d) not sizeof(struct timeval)", len);
	}
	else
	{
		snprintf(strres, sizeof(strres), "%d sec, %d usec", (int)tvptr->tv_sec, (int)tvptr->tv_usec);
	}
	return(strres);
}

