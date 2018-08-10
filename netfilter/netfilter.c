#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter_ipv4.h>
#include <linux/netfilter.h>
#include <linux/skbuff.h>
#include <linux/udp.h>
#include <linux/ip.h>
#include <net/ip.h>
#include <net/net_namespace.h>

static char *pDeviceName = "wlp3s0";

/* This function to be called by hook. */
/*
 *Old API
 * static unsigned int hook_func (unsigned int hooknum,
 *                                struct sk_buff *skb,
 *                                const struct net_device *in,
 *                                const struct net_device *out,
 *                                int (*okfn) (struct sk_buff *)
 *                              )
 */

MODULE_LICENSE("GPL");

static unsigned int hook_func(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{
    struct udphdr *udp_header;
    struct iphdr *ip_header = (struct iphdr *)skb_network_header(skb);

    if (ip_header->protocol == 17) {
        udp_header = (struct udphdr *)(skb_transport_header(skb) + ip_hdrlen(skb));
        printk(KERN_INFO "SRC: (%pI4):%d --> DST: (%pI4):%d\n", &ip_header->saddr, ntohs(udp_header->source), &ip_header->daddr, ntohs(udp_header->dest));
        printk(KERN_INFO "Drop udp packet.\n");

        return NF_DROP;
    }

    return NF_ACCEPT;
}

/*
static struct nf_hook_ops nfho = {
    .hook       = hook_func,
    .hooknum    = NF_INET_LOCAL_IN, // 1 -> NF_IP_LOCAL_IN
    .pf         = PF_INET,
    .priority   = NF_IP_PRI_FIRST,
    .dev	= dev_get_by_name(&init_net, pDeviceName),
};
*/
static struct nf_hook_ops nfho;


static int __init init_nf(void)
{
    int ret;

    nfho.hook = hook_func;
    nfho.hooknum = NF_INET_LOCAL_IN;
    nfho.pf = PF_INET;
    nfho.priority = NF_IP_PRI_FIRST;
    //nfho.dev = dev_get_by_name(&init_net, pDeviceName);

    ret = nf_register_net_hook(&init_net, &nfho);
  
    if (ret) {
        printk(KERN_INFO "could not register netfilter hook\n");
	nf_unregister_net_hook(&init_net, &nfho);
	return -1;
    }

    printk(KERN_INFO "Register netfilter module.\n");

    return 0;
}

static void __exit exit_nf(void)
{
    printk(KERN_INFO "Unregister netfilter module.\n");
    nf_unregister_net_hook(&init_net, &nfho);
}

module_init(init_nf);
module_exit(exit_nf);
