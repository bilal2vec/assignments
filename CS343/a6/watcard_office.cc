#include "watcard_office.h"

#include "bank.h"
#include "printer.h"

Args::Args(unsigned int s_id, unsigned int amount, WATCard* watcard)
    : s_id(s_id), amount(amount), watcard(watcard) {}

WATCardOffice::Job::Job(Args args) : args(args) {}

WATCardOffice::Courier::Courier(unsigned int id, Printer& printer, Bank& bank,
                                WATCardOffice* office)
    : id(id), printer(printer), bank(bank), office(office) {}

void WATCardOffice::Courier::main() {
  printer.print(Printer::Kind::Courier, id, 'S');

  while (true) {
    Job* job = office->requestWork();

    // WATCardOffice::requestWork() returns null when all jobs have been
    // completed, gracefully exit
    if (job == NULL) {
      break;
    }

    printer.print(Printer::Kind::Courier, id, 't', job->args.s_id,
                  job->args.amount);

    // Withdraw amount from student's bank account
    bank.withdraw(job->args.s_id, job->args.amount);

    // Deposit amount into watcard
    job->args.watcard->deposit(job->args.amount);

    if (prng(6) == 0) {
      // Lose watcard 1/6 of the time, raise exception
      printer.print(Printer::Kind::Courier, id, 'L', job->args.s_id);
      job->result.delivery(new WATCardOffice::Lost());
    } else {
      printer.print(Printer::Kind::Courier, id, 'T', job->args.s_id,
                    job->args.amount);
      job->result.delivery(job->args.watcard);
    }

    // Clean up job, we've already popped it from the queue so noone else is
    // going to clean this up
    delete job;
  }

  printer.print(Printer::Kind::Courier, id, 'F');
}

void WATCardOffice::main() {
  printer.print(Printer::Kind::WATCardOffice, 'S');

  while (true) {
    // Exit if destructor called
    _Accept(~WATCardOffice) {
      // Clean up any remaining jobs
      while (!(jobs.empty())) {
        delete jobs.front();
        jobs.pop();
      }

      // Unblock any couriers so courier tasks can exit gracefully
      for (unsigned int i = 0; i < num_couriers; i++) {
        _Accept(requestWork) {
          printer.print(Printer::Kind::WATCardOffice, 'W');
        }
      }

      break;
    }
    // Block any requests from couriers unless a Job request is ready
    or _When(!(jobs.empty())) _Accept(requestWork) {
      printer.print(Printer::Kind::WATCardOffice, 'W');
    }
    // Always let create and transfer requests in
    or _Accept(create){} or _Accept(transfer) {}
  }

  printer.print(Printer::Kind::WATCardOffice, 'F');
}

WATCardOffice::WATCardOffice(Printer& prt, Bank& bank, unsigned int numCouriers)
    : printer(prt), bank(bank), num_couriers(numCouriers) {
  // Create couriers
  couriers = new Courier*[numCouriers];
  for (unsigned int i = 0; i < numCouriers; i++) {
    couriers[i] = new Courier(i, printer, bank, this);
  }
}

WATCardOffice::~WATCardOffice() {
  // Delete couriers
  for (unsigned int i = 0; i < num_couriers; i++) {
    delete couriers[i];
  }
  delete[] couriers;

  // Delete any watcards that were lost
  for (unsigned int i = 0; i < watcards.size(); i++) {
    delete watcards[i];
  }
}

WATCard::FWATCard WATCardOffice::create(unsigned int sid, unsigned int amount) {
  // Create a watcard
  WATCard* watcard = new WATCard();

  // Push it to the vector of watcards so we don't lose its ptr
  watcards.push_back(watcard);

  // Create a job and push it to the queue
  Args job_args(sid, amount, watcard);
  Job* job = new Job(job_args);
  jobs.push(job);

  printer.print(Printer::Kind::WATCardOffice, 'C', sid, amount);

  // Return the future
  return job->result;
}

WATCard::FWATCard WATCardOffice::transfer(unsigned int sid, unsigned int amount,
                                          WATCard* card) {
  // Create a job and push it to the queue
  Args job_args(sid, amount, card);
  Job* job = new Job(job_args);
  jobs.push(job);

  printer.print(Printer::Kind::WATCardOffice, 'T', sid, amount);
  return job->result;
}

WATCardOffice::Job* WATCardOffice::requestWork() {
  // Return next job to service, send null when all jobs have been completed and
  // as signal to gracefully stop all couriers
  if (jobs.empty()) {
    return NULL;
  } else {
    Job* job = jobs.front();
    jobs.pop();
    return job;
  }
}