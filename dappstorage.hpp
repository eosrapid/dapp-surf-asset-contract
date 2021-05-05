#include <eosio/eosio.hpp>
#include <eosio/system.hpp>
#include <eosio/crypto.hpp>

using namespace eosio;

CONTRACT dappstorage : public contract {
  public:
    using contract::contract;
    dappstorage( name receiver, name code, datastream<const char*> ds )
      : contract(receiver, code, ds), tbl_resources(receiver, receiver.value) {}

    ACTION add(name uploader, checksum256 sha256hash, std::string data);

    TABLE s_tbl_resources {
      uint64_t rid;
      name uploader;
      checksum256 sha256hash;
      std::string data;
      uint64_t primary_key()const { return rid; }
      checksum256 by_hash()const { return sha256hash; }
    };

    typedef eosio::multi_index<"resources"_n, s_tbl_resources, eosio::indexed_by<"datahashidx"_n, eosio::const_mem_fun<s_tbl_resources, checksum256, &s_tbl_resources::by_hash>>> t_tbl_resources;

    using add_action = action_wrapper<"add"_n, &dappstorage::add>;
    t_tbl_resources tbl_resources;
};
